#include <iomanip>

#include "account.h"
#include "log.h"

LogGroup::LogGroup()
{
    std::ifstream tester("log");
    if (tester.good()) { // such file exists
        tester.close();
        _logs.open("log");
    } else { // no such file
        tester.close();

        // create file
        std::ofstream fileCreator("log");
        fileCreator.close();

        // open the file
        _logs.open("log");
    }

    tester.open("finance_log");
    if (tester.good()) { // such file exists
        tester.close();
        _finance_logs.open("finance_log");
    } else { // no such file
        tester.close();

        // create file
        std::ofstream fileCreator("finance_log");
        fileCreator.close();

        // open the file
        _finance_logs.open("finance_log");
    }
}

void LogGroup::addFinanceLog(FinanceLog& newLog)
{
    _finance_logs.seekp(0, std::ios::end);
    _finance_logs.write(reinterpret_cast<const char*>(&newLog), sizeof(FinanceLog));
}

void LogGroup::show(TokenScanner& line, const LoggingSituation& loggingStatus)
{
    if (loggingStatus.getPriority() < 7) throw InvalidCommand("Invalid");

    // skip the word "finance"
    line.nextToken();

    if (line.hasMoreToken()) {
        string_t limitString = line.nextToken();
        if (line.hasMoreToken()) throw InvalidCommand("Invalid");
        int limit = stringToInt(limitString);
        if (limit == 0) {
            std::cout << std::endl;
            return;
        }
        _finance_logs.seekp(0, std::ios::end);
        const int end = _finance_logs.tellp();
        if (end / sizeof(FinanceLog) < limit) throw InvalidCommand("Invalid");
        FinanceLog financeLog;
        std::cout << std::fixed << std::setprecision(2);
        for (int i = end - limit * sizeof(FinanceLog); i < end; i += sizeof(FinanceLog)) {
            _finance_logs.seekg(i);
            _finance_logs.read(reinterpret_cast<char*>(&financeLog), sizeof(FinanceLog));
            if (financeLog.flag) {
                std::cout << "+ " << financeLog.sum << " ";
            } else {
                std::cout << "- " << financeLog.sum << " ";
            }
        }
        std::cout << std::endl;
    } else {
        _finance_logs.seekp(0, std::ios::end);
        const int end = _finance_logs.tellp();
        FinanceLog financeLog;
        std::cout << std::fixed << std::setprecision(2);
        for (int i = 0; i < end; i += sizeof(FinanceLog)) {
            _finance_logs.seekg(i);
            _finance_logs.read(reinterpret_cast<char*>(&financeLog), sizeof(FinanceLog));
            if (financeLog.flag) {
                std::cout << "+ " << financeLog.sum << " ";
            } else {
                std::cout << "- " << financeLog.sum << " ";
            }
        }
        std::cout << std::endl;
    }
}
