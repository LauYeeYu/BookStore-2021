// Authored by Lau Yee-Yu
// Using ISO CPP standard

#include <iostream>

#include "exception.h"
#include "token_scanner.h"
#include "account.h"
#include "book.h"
#include "log.h"

void processLine(AccountGroup& accounts, BookGroup& books,
                 LogGroup& logs, LoggingSituation& logInStack);

void flush(AccountGroup& accounts, BookGroup& books, LogGroup& logs);

void init();

int main()
{
    init();
    AccountGroup accounts;
    BookGroup books;
    LogGroup logs;
    LoggingSituation logInStack;
    while (true) {
        try {
            processLine(accounts, books, logs, logInStack);
        } catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
    }
}

void processLine(AccountGroup& accounts, BookGroup& books,
                 LogGroup& logs, LoggingSituation& logInStack)
{
    TokenScanner line;
    line.newLine();

    if (!line.hasMoreToken()) throw InvalidCommand("Invalid");

    string_t command = line.nextToken();
    if (command == "quit" || command == "exit") {
        flush(accounts, books, logs);
        exit(0);
    } else if (command == "su") {
        accounts.switchUser(line, logInStack);
    } else if (command == "logout") {
        logInStack.logOut();
    } else if (command == "register") {
        accounts.registerUser(line);
    } else if (command == "passwd") {
        accounts.changePassword(line, logInStack);
    } else if (command == "useradd") {
        accounts.addUser(line, logInStack);
    } else if (command == "delete") {
        accounts.deleteUser(line, logInStack);
    } else if (command == "show") {
        books.show(line, logInStack, logs);
    } else if (command == "buy") {
        books.buy(line, logInStack, logs);
    } else if (command == "select") {
        books.select(line, logInStack, logs);
    } else if (command == "modify") {
        books.modify(line, logInStack, logs);
    } else if (command == "import") {
        books.importBook(line, logInStack, logs);
    } else if (command == "report") {
        //logs.report(line, logInStack);
    } else if (command == "log") {
        //logs.showLog(line);
    } else {
        throw InvalidCommand("Invalid");
    }
}

void init()
{
    std::ifstream tester("account_index");
    if (!(tester.good())) {
        std::ofstream creator("account_index");
        creator.close();
    }
    tester.close();

    tester.open("book_index_ISBN");
    if (!(tester.good())) {
        std::ofstream creator("book_index_ISBN");
        creator.close();
    }
    tester.close();

    tester.open("book_index_author");
    if (!(tester.good())) {
        std::ofstream creator("book_index_author");
        creator.close();
    }
    tester.close();

    tester.open("book_index_name");
    if (!(tester.good())) {
        std::ofstream creator("book_index_name");
        creator.close();
    }
    tester.close();

    tester.open("book_index_keyword");
    if (!(tester.good())) {
        std::ofstream creator("book_index_keyword");
        creator.close();
    }
    tester.close();
}

void flush(AccountGroup& accounts, BookGroup& books, LogGroup& logs)
{
    accounts.flush();
    books.flush();
    logs.flush();
}
