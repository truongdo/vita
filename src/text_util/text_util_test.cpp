/*
 * text_util_test.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include <text_util.h>
#include <iostream>

int main(){
    std::string test1 = "this is not good";
    std::string test2 = "abc";
    std::string line;
    while(getline(std::cin, line)){
        normalized_number(line);
        std::cout << line << std::endl;
    }
}

