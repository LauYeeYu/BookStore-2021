#include "token_scanner.h"

std::string TokenScanner::nextToken()
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
