#ifndef UNROLLED_LINKED_LIST
#define UNROLLED_LINKED_LIST

#include <fstream>
#include <vector>

/**
 * @class UnrolledLinkedList
 *
 * This is a template class of double unrolled linked list on disk.
 * <br><br>
 * WARNING: the key type MUST have valid operator< and operator== !
 * @tparam keyType Type of Key
 * @tparam valueType Type of Value
 */
template <class keyType, class valueType>
class UnrolledLinkedList {
private:
    typedef int ptr;

    std::fstream _list;

    /// The following are private components of this linked list

    /**
     * @struct _first_node {next, pre, nodeSize, maxNodeSize, size}
     *
     * This is the node to pointer to the data, and metadata of the list
     */
    struct _first_node {
        ptr next; // the first main node

        ptr pre; // the last main node

        int nodeSize;

        int maxNodeSize;

        int size;
    } _head;

    /**
     * @struct _main_node {key, value, target, count, next, pre}
     *
     * This is the node to store the pointer and data, and
     * link other main nodes.
     */
    struct _main_node {
        keyType key;

        valueType value;

        ptr target;

        int count;

        ptr next;

        ptr pre;
    };

    /**
     * @struct _node {key, value}
     *
     * This is the Node to store data.
     */
    struct _node {
        keyType key;

        valueType value;
    } _empty_node;

    /**
     * This function return a pair of the pointer to the main node
     * and the number of node the target is right after, for the case
     * of the main node, the second member of the pair is -1.  If the
     * key doesn't belongs to the unrolled linked list, it will the
     * node ahead of its place (except the case that the key is less than
     * any other keys, and in such case, it will return the first main node).
     * @param key
     * @return a pair of the pointer to the main node and the offset
     */
    std::pair<ptr, int> _find(const keyType& key)
    {
        if (_head.next != 0) {
            _main_node tmp;
            ptr Ptr = _head.pre;
            _list.seekg(_head.pre);
            _list.read(reinterpret_cast<char *>(&tmp), sizeof(_main_node));

            // Searching for the approximate place (only the main node)
            while (key < tmp.key && tmp.pre != 0) {
                Ptr = tmp.pre;
                _list.seekg(Ptr);
                _list.read(reinterpret_cast<char *>(&tmp), sizeof(_main_node));
            }

            // Searching for the exact place
            if (tmp.pre == 0 && key < tmp.key) return std::make_pair(Ptr, -1);

            if (key == tmp.key || tmp.count == 0) return std::make_pair(Ptr, -1);

            _node tempNode;
            int leftIndex = 0, rightIndex = tmp.count - 1;

            // the case that the key is between the main node and the first node
            _list.seekg(tmp.target);
            _list.read(reinterpret_cast<char*>(&tempNode), sizeof(_node));
            if (key < tempNode.key || key == tempNode.key) return std::make_pair(Ptr, -1);

            // the case that the key is right after the last node
            _list.seekg(tmp.target + (tmp.count - 1) * sizeof(_node));
            _list.read(reinterpret_cast<char*>(&tempNode), sizeof(_node));
            if (tempNode.key < key || tempNode.key == key) return std::make_pair(Ptr, tmp.count - 1);

            while (rightIndex - leftIndex > 1) {
                _list.seekg(tmp.target + ((rightIndex + leftIndex) / 2) * sizeof(_node));
                _list.read(reinterpret_cast<char*>(&tempNode), sizeof(_node));
                if (key < tempNode.key) rightIndex = (rightIndex + leftIndex) / 2;
                else leftIndex = (rightIndex + leftIndex) / 2;
            }

            return std::make_pair(Ptr, leftIndex);
        } else {
            return std::make_pair(0, 0);
        }
    }

    /**
     * This function makes a new main node right after the target.
     * <br><br>
     * WARNING: If the target is 0, that means its a empty list.
     * @param mainNode the new main node to be added to the unrolled
     * linked list
     * @param target the place where the new node is added (it MUST
     * be a main node).
     * @return the new place of the new main node.
     */
    ptr _new_node(const _main_node& mainNode, ptr target)
    {
        if (target != 0) {
            _main_node pre, next;
            _list.seekg(target);
            _list.read(reinterpret_cast<char*>(&pre), sizeof(_main_node));
            if (pre.next != 0) {
                _list.seekg(pre.next);
                _list.read(reinterpret_cast<char *>(&next), sizeof(_main_node));
                _list.seekp(0, std::ios::end);
                mainNode.next = pre.next;
                mainNode.pre = next.pre;
                pre.next = _list.tellp();
                next.pre = _list.tellp();
                mainNode.target = _head.next + sizeof(_main_node);
                _list.write(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));
                _list.seekp(_head.maxNodeSize * sizeof(_node), std::ios::end);
                _list.write(reinterpret_cast<char*>(&_empty_node), sizeof(_node));
                _list.seekp(mainNode.next);
                _list.write(reinterpret_cast<char*>(&next), sizeof(_main_node));
                _list.seekp(mainNode.pre);
                _list.write(reinterpret_cast<char*>(&pre), sizeof(_main_node));
                return pre.next;
            } else {
                _list.seekp(0, std::ios::end);
                mainNode.next = 0;
                mainNode.pre = target;
                pre.next = _list.tellp();
                _head.pre = _list.tellp();
                mainNode.target = _head.next + sizeof(_main_node);
                _list.write(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));
                _list.seekp(_head.maxNodeSize * sizeof(_node), std::ios::end);
                _list.write(reinterpret_cast<char*>(&_empty_node), sizeof(_node));
                _list.seekp(mainNode.pre);
                _list.write(reinterpret_cast<char*>(&pre), sizeof(_main_node));
                _list.seekp(0);
                _list.write(reinterpret_cast<char*>(&_head), sizeof(_first_node));
                return pre.next;
            }
        } else {
            _list.seekp(0, std::ios::end);
            _head.next = _list.tellp();
            _head.pre = _list.tellp();
            mainNode.target = _head.next + sizeof(_main_node);
            mainNode.next = 0;
            mainNode.pre = 0;
            _list.write(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));

            // to reserve the space for the array of the main node
            _list.seekp(_head.maxNodeSize * sizeof(_node), std::ios::end);
            _list.write(reinterpret_cast<char*>(&_empty_node), sizeof(_node));
            _list.seekp(0);
            _list.write(reinterpret_cast<char*>(&_head), sizeof(_first_node));
            return _head.next;
        }
    }

    /**
     * This function split a main node into two.
     * @param mainNode
     * @param mainNodePtr
     * @return the ptr of the main node
     */
    ptr _split(_main_node& mainNode, ptr mainNodePtr)
    {
        // Copy the extra string of nodes
        auto* nodeBuffer = new _node[mainNode.count - _head.nodeSize];
        for (int i = 0; i < mainNode.count; ++i) {
            _list.seekg(mainNode.target + sizeof(_node) * (_head.nodeSize + i));
            _list.read(reinterpret_cast<char *>(&nodeBuffer[i]), sizeof(_node));
        }
        // Create a new node
        ptr newMainNodePtr;
        _main_node newMainNode {nodeBuffer[0].key, nodeBuffer[0].value, 0,
                                mainNode.count - _head.nodeSize - 1, 0, 0};

        // Get a new space for the nodes
        mainNode.count = _head.nodeSize;
        newMainNodePtr = _new_node(newMainNode, mainNodePtr);

        // Change the count of the original node
        _list.seekp(mainNodePtr);
        _list.write(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));

        // Get the new main node
        _list.seekg(newMainNodePtr);
        _list.read(reinterpret_cast<char*>(&newMainNode), sizeof(_main_node));

        // write the extra string of nodes
        _list.seekp(newMainNode.target);
        for (int i = 1; i <= newMainNode.count; ++i) {
            _list.write(reinterpret_cast<char*>(&nodeBuffer[i]), sizeof(_node));
        }
        delete[] nodeBuffer;
        return newMainNodePtr;
    }

public:
    explicit UnrolledLinkedList(const std::string& fileName, int nodeSize = 316)
    : _list(fileName), _head{0, 0, nodeSize, 2 * nodeSize, 0}
    {
        _list.seekg(0);
        _list.seekp(0);
        if (_list.peek() == EOF) {
            _list.seekg(0, std::ios::beg);
            _list.seekp(0, std::ios::beg);
            _list.write(reinterpret_cast<char*>(&_head), sizeof(_first_node));
        } else {
            _list.read(reinterpret_cast<char*>(&_head), sizeof(_first_node));
        }
    }

    ~UnrolledLinkedList() = default;

    void insert(const keyType& key, const valueType& value)
    {
        std::pair<ptr, int> position = _find(key);
        if (position.first == 0) {
            _main_node mainNode {key, value, 0, 0, 0, 0};
            _new_node(mainNode, 0);
            return;
        }
        _main_node mainNode; // the place to place the new node
        _list.seekg(position.first);
        _list.read(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));
        if (mainNode.count >= _head.maxNodeSize) {
            ptr newMainNode = _split(mainNode, position.first);
        }
        _node tmpNode;
        if (mainNode.pre == 0 && key < mainNode.key) {
            // Push back the data
            for (int i = _head.nodeSize; i > 0; --i) {
                _list.seekg(mainNode.target + (i - 1) * sizeof(_node));
                _list.read(reinterpret_cast<char*>(&tmpNode), sizeof(_node));
                _list.seekp(mainNode.target + i * sizeof(_node));
                _list.write(reinterpret_cast<char*>(&tmpNode), sizeof(_node));
            }

            // Move the data in main node to the first node of its array
            tmpNode.key = mainNode.key;
            tmpNode.value = mainNode.value;
            _list.seekp(mainNode.target);
            _list.write(reinterpret_cast<char*>(&tmpNode), sizeof(_node));

            // set the new data in the main node
            mainNode.key = key;
            mainNode.value = value;
            _list.seekp(position.first);
            _list.write(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));
        } else {
            //todo
        }
    }

    void pop(const keyType& key)
    {
        //todo
    }

    void modify(const keyType& key, const valueType& value)
    {
        //todo
    }

    void clear()
    {
        _list.clear();
        _head.size = 0;
    }

    valueType get(const keyType& key) const
    {
        //todo
    }

    std::vector<valueType> traverse() const
    {
        //todo
    }
};

/**
 * @class DoubleUnrolledLinkedList
 *
 * This is a template class of double unrolled linked list on disk.
 * <br><br>
 * WARNING: the key type MUST have valid operator< and operator== !
 *
 * @tparam keyType1 Type of First key
 * @tparam keyType2 Type of Second key
 * @tparam valueType Type of Value
 */
template <class keyType1, class keyType2, class valueType>
class DoubleUnrolledLinkedList {
private:
    typedef int ptr;

    std::fstream _list;

    /// The following are private components of this linked list

    /**
     * @struct _first_node
     *
     * This is the node to pointer to the data, and metadata of the list
     */
    struct _first_node {
        ptr next; // the first main node

        ptr pre; // the last main node

        int nodeSize;

        int maxNodeSize;

        int size;
    } _head;

    /**
     * @struct _main_node
     *
     * This is the node to store the pointer and data, and
     * link other main nodes.
     */
    struct _main_node {
        keyType1 key1;

        keyType2 key2;

        valueType value;

        ptr target;

        int count;

        ptr next;

        ptr pre;
    };

    /**
     * @struct _node
     *
     * This is the Node to store data.
     */
    struct _node {
        keyType1 key1;

        keyType2 key2;

        valueType value;
    } _empty_node;

    /**
     * This function return a pair of the pointer to the main node
     * and the number of node the target is right after, for the case
     * of the main node, the second member of the pair is -1.  If the
     * key doesn't belongs to the unrolled linked list, it will the
     * node ahead of its place (except the case that the key is less than
     * any other keys, and in such case, it will return the first main node).
     * @param key
     * @return a pair of the pointer to the main node and the offset
     */
    std::pair<ptr, int> _find(const keyType1& key1, const keyType2& key2)
    {
        if (_head.next != 0) {
            _main_node tmp;
            ptr Ptr = _head.pre;
            _list.seekg(_head.pre);
            _list.read(reinterpret_cast<char*>(&tmp), sizeof(_main_node));

            // Searching for the approximate place (only the main node)
            while (key1 < tmp.key1 && key2 < tmp.key2 && tmp.pre != 0) {
                Ptr = tmp.pre;
                _list.seekg(Ptr);
                _list.read(reinterpret_cast<char*>(&tmp), sizeof(_main_node));
            }

            // Searching for the exact place

            if (tmp.pre == 0 && (key1 < tmp.key1 || key2 < tmp.key2)) return std::make_pair(Ptr, -1);

            if (key1 == tmp.key1 && key2 == tmp.key2 || tmp.count == 0) return std::make_pair(Ptr, -1);

            _node tempNode;
            int leftIndex = 0, rightIndex = tmp.count - 1;

            // the case that the key is between the main node and the first node
            _list.seekg(tmp.target);
            _list.read(reinterpret_cast<char*>(&tempNode), sizeof(_node));
            if ((key1 < tempNode.key1 || key1 == tempNode.key1) && (key2 < tempNode.key2 || key2 == tempNode.key2)) {
                return std::make_pair(Ptr, -1);
            }

            // the case that the key is right after the last node
            _list.seekg(tmp.target + (tmp.count - 1) * sizeof(_node));
            _list.read(reinterpret_cast<char*>(&tempNode), sizeof(_node));
            if ((tempNode.key1 < key1 || tempNode.key1 == key1) && (key2 < tempNode.key2 || key2 == tempNode.key2)) {
                return std::make_pair(Ptr, tmp.count - 1);
            }

            while (rightIndex - leftIndex > 1) {
                _list.seekg(tmp.target + ((rightIndex + leftIndex) / 2) * sizeof(_node));
                _list.read(reinterpret_cast<char*>(&tempNode), sizeof(_node));
                if (key1 < tempNode.key1 && key2 < tempNode.key2) rightIndex = (rightIndex + leftIndex) / 2;
                else leftIndex = (rightIndex + leftIndex) / 2;
            }

            return std::make_pair(Ptr, leftIndex);
        } else {
            return std::make_pair(0, 0);
        }
    }

    /**
     * This function makes a new main node right after the target.
     * <br><br>
     * WARNING: If the target is 0, that means its a empty list.
     * @param mainNode the new main node to be added to the unrolled
     * linked list
     * @param target the place where the new node is added (it MUST
     * be a main node).
     * @return the new place of the new main node.
     */
    ptr _new_node(const _main_node& mainNode, ptr target)
    {
        if (target != 0) {
            _main_node pre, next;
            _list.seekg(target);
            _list.read(reinterpret_cast<char*>(&pre), sizeof(_main_node));
            if (pre.next != 0) {
                _list.seekg(pre.next);
                _list.read(reinterpret_cast<char *>(&next), sizeof(_main_node));
                _list.seekp(0, std::ios::end);
                mainNode.next = pre.next;
                mainNode.pre = next.pre;
                pre.next = _list.tellp();
                next.pre = _list.tellp();
                mainNode.target = _head.next + sizeof(_main_node);
                _list.write(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));
                _list.seekp(_head.maxNodeSize * sizeof(_node), std::ios::end);
                _list.write(reinterpret_cast<char*>(&_empty_node), sizeof(_node));
                _list.seekp(mainNode.next);
                _list.write(reinterpret_cast<char*>(&next), sizeof(_main_node));
                _list.seekp(mainNode.pre);
                _list.write(reinterpret_cast<char*>(&pre), sizeof(_main_node));
                return pre.next;
            } else {
                _list.seekp(0, std::ios::end);
                mainNode.next = 0;
                mainNode.pre = target;
                pre.next = _list.tellp();
                _head.pre = _list.tellp();
                mainNode.target = _head.next + sizeof(_main_node);
                _list.write(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));
                _list.seekp(_head.maxNodeSize * sizeof(_node), std::ios::end);
                _list.write(reinterpret_cast<char*>(&_empty_node), sizeof(_node));
                _list.seekp(mainNode.pre);
                _list.write(reinterpret_cast<char*>(&pre), sizeof(_main_node));
                _list.seekp(0);
                _list.write(reinterpret_cast<char*>(&_head), sizeof(_first_node));
                return pre.next;
            }
        } else {
            _list.seekp(0, std::ios::end);
            _head.next = _list.tellp();
            _head.pre = _list.tellp();
            mainNode.target = _head.next + sizeof(_main_node);
            mainNode.next = 0;
            mainNode.pre = 0;
            _list.write(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));

            // to reserve the space for the array of the main node
            _list.seekp(_head.maxNodeSize * sizeof(_node), std::ios::end);
            _list.write(reinterpret_cast<char*>(&_empty_node), sizeof(_node));
            _list.seekp(0);
            _list.write(reinterpret_cast<char*>(&_head), sizeof(_first_node));
            return _head.next;
        }
    }

    /**
     * This function split a main node into two.
     * @param mainNode
     * @param mainNodePtr
     * @return the ptr of the main node
     */
    ptr _split(_main_node& mainNode, ptr mainNodePtr)
    {
        // Copy the extra string of nodes
        auto* nodeBuffer = new _node[mainNode.count - _head.nodeSize];
        for (int i = 0; i < mainNode.count; ++i) {
            _list.seekg(mainNode.target + sizeof(_node) * (_head.nodeSize + i));
            _list.read(reinterpret_cast<char *>(&nodeBuffer[i]), sizeof(_node));
        }
        // Create a new node
        ptr newMainNodePtr;
        _main_node newMainNode {nodeBuffer[0].key, nodeBuffer[0].value, 0,
                                mainNode.count - _head.nodeSize - 1, 0, 0};

        // Get a new space for the nodes
        mainNode.count = _head.nodeSize;
        newMainNodePtr = _new_node(newMainNode, mainNodePtr);

        // Change the count of the original node
        _list.seekp(mainNodePtr);
        _list.write(reinterpret_cast<char*>(&mainNode), sizeof(_main_node));

        // Get the new main node
        _list.seekg(newMainNodePtr);
        _list.read(reinterpret_cast<char*>(&newMainNode), sizeof(_main_node));

        // write the extra string of nodes
        _list.seekp(newMainNode.target);
        for (int i = 1; i <= newMainNode.count; ++i) {
            _list.write(reinterpret_cast<char*>(&nodeBuffer[i]), sizeof(_node));
        }
        delete[] nodeBuffer;
        return newMainNodePtr;
    }

public:
    explicit DoubleUnrolledLinkedList(const std::string& fileName, int nodeSize = 316)
    : _list(fileName) {}

    ~DoubleUnrolledLinkedList() = default;

    void insert(const keyType1& key1, const keyType2& key2,
                const valueType& value)
    {
        //todo
    }

    void pop(const keyType1& key1, const keyType2& key2,
             const valueType& value)
    {
        //todo
    }

    void modify(const keyType1& key1, const keyType2& key2,
                const valueType& value)
    {
        //todo
    }

    void clear()
    {
        _list.clear();
        _head.size = 0;
    }

    valueType get(const keyType1& key1, const keyType2& key2) const
    {
        //todo
    }

    std::vector<valueType> traverse() const
    {
        //todo
    }

    std::vector<valueType> traverse(const keyType1& key1) const
    {
        //todo
    }
};

#endif // UNROLLED_LINKED_LIST
