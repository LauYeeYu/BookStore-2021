#ifndef TOKEN_SCANNER
#define TOKEN_SCANNER

#include <string>
#include <utility>

typedef char char_t;
typedef std::string string_t;

class TokenScanner {
private:
    string_t _buffer;
    char _delimiter = ' ';
    int _current = 0;
public:
    TokenScanner() = default;

    ~TokenScanner() = default;

    explicit TokenScanner(string_t input, char delimiter = ' ')
    : _buffer(std::move(input)), _delimiter(delimiter) {}

    string_t nextToken();

    bool hasMoreToken();
};

#endif //TOKEN_SCANNER
