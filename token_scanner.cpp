#include "token_scanner.h"

string_t TokenScanner::nextToken()
{
    // Skip delimiter
    while (_current < _buffer.size() && _buffer[_current] == _delimiter) ++_current;
    int start = _current;

    // Find another delimiter
    while (_current < _buffer.size() && _buffer[_current] != _delimiter) ++_current;
    return _buffer.substr(start, _current - start);
}

bool TokenScanner::hasMoreToken()
{
    while (_current < _buffer.size() && _buffer[_current] == _delimiter) ++_current;
    return _current != _buffer.size();
}

std::istream& operator>>(std::istream& is, TokenScanner& obj)
{
    obj._current = 0;
    is >> obj._buffer;
    return is;
}

int stringToInt(const string_t& input)
{
    int output = 0;
    for (const char c : input) {
        if (c < 48 || c > 57) throw InvalidCommand("Invalid");
        output = output * 10 + c - 48;
    }
    return output;
}

double stringToDouble(const string_t& input)
{
    double output = 0;
    bool point = false;
    double number = 1;
    for (const char c : input) {
        if (c < 46 || c == 47 || c > 57) throw InvalidCommand("Invalid");
        if (point) {
            if (c == '.') throw InvalidCommand("Invalid");
            number *= 10;
            output += double(c - 48) / number;
        } else {
            if (c == '.') {
                point = true;
                continue;
            }
            output = output * 10 + c - 48;
        }
    }
    return output;
}