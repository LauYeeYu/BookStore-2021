#ifndef TOKEN_SCANNER
#define TOKEN_SCANNER

#include <string>
#include <utility>

#include "exception.h"

typedef char char_t;
typedef std::string string_t;

int stringToInt(const string_t& input);
double stringToDouble(const string_t& input);

class TokenScanner {
private:
    string_t _buffer;
    char_t _delimiter = ' ';
    int _current = 0;
public:
    TokenScanner() = default;

    ~TokenScanner() = default;

    explicit TokenScanner(string_t input, char_t delimiter = ' ')
    : _buffer(std::move(input)), _delimiter(delimiter) {}

    void newLine();

    string_t nextToken();

    bool hasMoreToken();
};

#endif //TOKEN_SCANNER
