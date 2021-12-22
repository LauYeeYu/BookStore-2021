// Authored by Lau Yee-Yu
// Using ISO CPP standard

#include <iostream>
#include <fstream>
#include <iomanip>

#include "exception.h"
#include "unrolled_linked_list.h"
#include "token_scanner.h"

int main()
{
    while (true) {
        try {
            exit(0);
        } catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
    }
    return 0;
}
