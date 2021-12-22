#ifndef LOG
#define LOG

#include <iostream>
#include <fstream>

#include "Unrolled_Linked_list.h"
#include "token_scanner.h"
#include "account.h"

struct log {
    int num;
    bool seal;
    char operateAccount[31];
    char bookName[61];
};

class LogGroup {
private:
    std::fstream _logs;

public:
    LogGroup();

    ~LogGroup();

    void report(TokenScanner& line, const LoggingSituation& loggingStatus);

    void add(log& newLog, const LoggingSituation& loggingStatus);

    void show(TokenScanner& line, const LoggingSituation& loggingStatus);

    void reportFinance();

    void reportEmployee();
};

#endif //LOG
