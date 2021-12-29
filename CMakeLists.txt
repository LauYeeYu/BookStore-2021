cmake_minimum_required(VERSION 3.16)
project(Bookstore)

set(CMAKE_CXX_STANDARD 17)

add_executable(Bookstore
        bookstore_main.cpp
        unrolled_linked_list.h
        token_scanner.h
        token_scanner.cpp
        account.h
        account.cpp
        exception.h
        book.h
        book.cpp
        log.h
        log.cpp)