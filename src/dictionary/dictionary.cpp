/*
 * dictionary.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include "dictionary.h"
#include <text_util.h>

Dictionary::Dictionary(){}
Dictionary::~Dictionary(){}
int Dictionary::LoadDict(std::string fname){
    std::ifstream infile(fname.c_str());
    if (! infile){
        std::cerr << "File " << fname << " does not exists" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> line_parts;
    const char* delim = " ";
    while (getline(infile, line)){
        SplitStringToVector(line, delim, true, &line_parts);
        if (line_parts.size() == 1){
            std::cout << "This line is not good " << line << std::endl;
            continue;
        }
        std::string phone = "";
        std::string tone = line_parts[1];
        for (std::size_t i = 2; i < line_parts.size(); i++) {
            if (i > 2)
                phone += " ";
            phone += line_parts[i];
        }
        this->_dict[line_parts[0]].push_back(tone); // Tone
        this->_dict[line_parts[0]].push_back(phone);    // Phoneme
    }
    return 0;
}

void Dictionary::PrintDict(){
    for (std::map<std::string, std::vector<std::string> >::iterator item=this->_dict.begin(); item!=this->_dict.end(); ++item){
        std::cout << item->first <<std::endl;
    }
}

void Dictionary::AddWord(std::string word, std::vector<std::string> phones){
    this->_dict[word] = phones;
}

void Dictionary::GetPhoneme(std::string key,
        std::string *tone, std::string *phone_list, bool out_tone){
    std::map<std::string, std::vector<std::string> >::iterator it;
    std::vector<std::string> words;
    const char* delim = "_";
    SplitStringToVector(key, delim, true, &words);

    for (std::size_t idx = 0; idx != words.size(); ++idx){
        std::string norm_w = words[idx];
        // Normalizes some characters because they can not be lowered
        ReplaceStringInPlace(norm_w, "Đ", "đ");
        // Converts everything to lowercase
        std::transform(norm_w.begin(), norm_w.end(), norm_w.begin(), ::tolower);
        it = this->_dict.find(norm_w);
        if (it != this->_dict.end()){
            if (idx > 0){
                (*tone) += "_";
                (*phone_list) += "_";
            }
            (*tone) += it->second[0];
            if (out_tone){
                const char* delim2 = " ";
                std::vector<std::string> phones;
                SplitStringToVector(it->second[1], delim2, true, &phones);
                std::vector<std::string>::iterator it_phone = phones.begin();
                for (; it_phone != phones.end(); ++it_phone){
                    (*phone_list) += (*it_phone) + it->second[0] + " ";
                }
            }
            else{
                (*phone_list) += it->second[1];
            }
        }else{

        }

    }
}

