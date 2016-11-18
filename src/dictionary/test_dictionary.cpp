/*
 * test_dictionary.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include <dictionary.h>


int main(){
    Dictionary dict = Dictionary();
    dict.LoadDict("/home/truong-d/CloudStation/work/openvtalk/data/models/vn.dict");
    std::string tone, phoneme;
    dict.GetPhoneme("xin", &tone, &phoneme, false);
}
