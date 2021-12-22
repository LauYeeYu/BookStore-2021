#ifndef BOOK
#define BOOK

#include <iostream>
#include <fstream>
#include <vector>

#include "unrolled_linked_list.h"
#include "token_scanner.h"
#include "account.h"
#include "log.h"

struct ISBN {
    char isbn[21];

    ISBN(const std::string& isbn_in);

    bool operator==(const ISBN& rhs) const;

    bool operator<(const ISBN& rhs) const;
};

struct Name {
    char name[61];

    Name(const std::string& name_in);

    bool operator==(const Name& rhs) const;

    bool operator<(const Name& rhs) const;
};

struct Author {
    char author[61];

    Author(const std::string& author_in);

    bool operator==(const Author&) const;

    bool operator<(const Author&) const;
};

struct Keyword {
    char keyword[61];

    Keyword(const std::string& ketword_in);

    bool operator==(const Keyword&) const;

    bool operator<(const Keyword&) const;
};

struct Keywords {
    char keywords[61];

    Keywords(const std::string& ketword_in);

    bool operator==(const Keywords&) const;

    bool operator<(const Keywords&) const;
};

struct Book {
    int id;

    ISBN isbn;

    Name name;

    Author author;

    Keywords keywords;

    int quantity = -1;

    double price = -1;

    double total_cost = -1;

    Book(int id, const std::string& isbn_in, const std::string& name, const std::string& Author, const std::string& keywords, int quantity, double price, double cost);

    Book(int id, const std::string& isbn_in)

    friend str::ostream& operator<<(std::ostream& os, const Book& book);
};

class BookGroup {
private:
    UnrolledLinkedList<ISBN, int> _isbn_book_map
    = UnrolledLinkedList<ISBN, int>("book_index_ISBN");

    DoubleUnrolledLinkedList<Name, ISBN, int> _name_book_map
    = DoubleUnrolledLinkedList<Name, ISBN, int>("book_index_name");

    DoubleUnrolledLinkedList<Author, ISBN, int> _author_book_map
    = DoubleUnrolledLinkedList<Author, ISBN, int>("book_index_name");

    DoubleUnrolledLinkedList<Keyword, ISBN, int> _keywords_book_map
    = DoubleUnrolledLinkedList<Keyword, ISBN, int>("book_index_name");

    int all_book_num = 0;

public:
    BookGroup();

    ~BookGroup();

    Book find(TokenScanner& line, const LoggingSituation& loggingStatus, LogGroup& logGroup);

    void change(TokenScanner& line, const LoggingSituation& loggingStatus, LogGroup& logGroup);

    void output(TokenScanner& line, const LoggingSituation& loggingStatus, LogGroup& logGroup);

    void input(TokenScanner& line, const LoggingSituation& loggingStatus, LogGroup& logGroup);

    void select(TokenScanner& line, LoggingSituation& loggingStatus, LogGroup& logGroup);
};

#endif //BOOK
