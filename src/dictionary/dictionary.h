/*
 * dictionary.h
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <cstdlib>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

class Dictionary{
    private:
        std::map<std::string, std::vector<std::string> > _dict;

    public:
        Dictionary();
        ~Dictionary();
        int LoadDict(std::string fname);
        void PrintDict();
        void AddWord(std::string word, std::vector<std::string> phones);
        // The first value is tone, second is phoneme list
        void GetPhoneme(std::string key, std::string *tone, std::string *phone_list, bool out_tone);
};
#endif /* !DICTIONARY_H */
