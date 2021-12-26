#ifndef LOG
#define LOG

#include <iostream>
#include <fstream>

#include "unrolled_linked_list.h"
#include "token_scanner.h"

struct FinanceLog {
    double sum;

    bool flag; // true to be income and false to be expenditure
};

struct Log {
    double sum;

    bool flag; // true to be income and false to be expenditure

    UserID userID;

    char bookName[61];
};

class LogGroup {
private:
    std::fstream _logs;

    std::fstream _finance_logs;

public:
    LogGroup();

    ~LogGroup() = default;

    void report(TokenScanner& line, const LoggingSituation& loggingStatus);

    void add(Log& newLog, const LoggingSituation& loggingStatus);

    void addFinanceLog(FinanceLog& newLog);

    /**
     * this function
     * @param line
     * @param loggingStatus
     */
    void show(TokenScanner& line, const LoggingSituation& loggingStatus);

    void reportFinance();

    void reportEmployee();

    void showLog(TokenScanner& line);

    void flush();
};

#endif //LOG
