/*
 * logging.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>

inline void OpenVtalk_ERROR(std::string filename, int line, std::string message){
    std::cout << filename << ":"<<line<< ":" << message << std::endl;
    exit(1);
}

inline void OpenVtalk_WARN(std::string filename, int line, std::string message){
    std::cout << filename << ":"<<line<< ":" << message << std::endl;
}
