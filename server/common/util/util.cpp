//
// Created by 25421 on 2024/4/24.
//

#include "util.h"
#include <iostream>

void errif(bool exp, const char* s) {
    if (exp) {
        std::cout << s << std::endl;
    }
}