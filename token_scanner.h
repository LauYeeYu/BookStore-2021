#ifndef TOKEN_SCANNER
#define TOKEN_SCANNER

#include <string>
#include <utility>

class TokenScanner {
private:
    std::string _buffer;
    char _delimiter = ' ';
    int _current = 0;
public:
    TokenScanner() = default;

    ~TokenScanner() = default;

    explicit TokenScanner(std::string input, char delimiter = ' ')
    : _buffer(std::move(input)), _delimiter(delimiter) {}

    std::string nextToken();

    bool hasMoreToken();
};

#endif //TOKEN_SCANNER
