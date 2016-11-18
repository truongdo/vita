/*
 * text_util.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include "text_util.h"
std::map<int, std::string> num2text = {
        {0, "không"},
        {1, "một"},
        {2, "hai"},
        {3, "ba"},
        {4, "bốn"},
        {5, "năm"},
        {6, "sáu"},
        {7, "bảy"},
        {8, "tám"},
        {9, "chín"},
        {10, "mười"},
    };

void SplitStringToVector(const std::string &full, const char *delim,
        bool omit_empty_strings,
        std::vector<std::string> *out){
    size_t start = 0, found = 0, end = full.size();
    out->clear();
    while (found != std::string::npos) {
        found = full.find_first_of(delim, start);
        // start != end condition is for when the delimiter is at the end
        if (!omit_empty_strings || (found != start && start != end))
            out->push_back(full.substr(start, found - start));
        start = found + 1;
    }
}

void SplitStringToVector(const std::string &full, std::vector<std::string> *out){
    size_t start = 0, found = 0, end = full.size();
    const char *delim = " ";
    bool omit_empty_strings = true;
    out->clear();
    while (found != std::string::npos) {
        found = full.find_first_of(delim, start);
        // start != end condition is for when the delimiter is at the end
        if (!omit_empty_strings || (found != start && start != end))
            out->push_back(full.substr(start, found - start));
        start = found + 1;
    }
}

void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

void clean(std::string &text){
    std::vector<std::string> punctuation = {"(", ")", "\"", "\'", "”", "“" , "–"};
    std::vector<std::string> periods = {".", ",", ";", "?", ":", "-"};
    std::vector<std::string>::iterator it;
    for (it = punctuation.begin(); it != punctuation.end(); ++it){
        ReplaceStringInPlace(text, *it, "");
    }

    for (it = periods.begin(); it != periods.end(); ++it){
        ReplaceStringInPlace(text, *it, " " + *it + " ");
    }
}

std::string conv_num2string(std::string num_str){
    std::string text;
    int num_len = num_str.size();
    int ori_len = num_len;
    for(char& c : num_str) {
        int c_int = c - '0';
        if (num_len == 5){
            text += num2text[c_int];
            text += "mươi ";
        }
        if (num_len == 4){
            if (c_int != 0)
                text += num2text[c_int];
            text += " nghìn ";
        }
        if (num_len == 3){
            text += num2text[c_int];
            text += " trăm ";
        }
        if (num_len == 2){
            if (ori_len > 2){
                if (c_int == 0)
                    text += " linh ";
                if (c_int == 1)
                    text += " mười ";
            }else{
                if (c_int == 1)
                    text += " mười ";
            }
            if (c_int > 1){
                text += num2text[c_int];
                text += " mươi ";
            }

        }
        if (num_len == 1){
            if (ori_len > 1){
                if (c_int == 1)
                    text += "một";
                else if (c_int == 0)
                    ReplaceStringInPlace(text, "linh", "");
                else
                    text += num2text[c_int];
            }else{
                text += num2text[c_int];
            }
        }
        num_len--;
    }
    return text;
}
std::string conv_num2day(std::string day_str){
    int day = std::atoi(day_str.c_str());
    int last_num = day_str.back() - '0';
    std::string text;
    if (day < 10){
        text = " mùng " + num2text[last_num];
    }else{
        if (day < 20){
            text = " mười " + num2text[last_num];
        }else{
            if (day < 30){
                text = " hai mươi " + num2text[last_num];
                ReplaceStringInPlace(text, "hai mươi một", "hai mươi mốt");
                ReplaceStringInPlace(text, "hai mươi không", "hai mươi");
            }else{
                text = " ba mươi " + num2text[last_num];
                ReplaceStringInPlace(text, "ba mươi không", "ba mươi");
            }
        }
    }
    return text;
}

std::string conv_num2month(std::string month_str){
    std::string text;
    int month = std::atoi(month_str.c_str());
    int last_num = month_str.back() - '0';
    if (month < 10){
        text = " tháng " + num2text[last_num];
    }else{
        text = " tháng mười " + num2text[last_num];

    }
    return text;
}

std::string conv_num2year(std::string year_str){
    std::string text = " năm ";
    for (char& c : year_str){
        int num = c - '0';
        text += num2text[num] + " ";
    }
    return text;
}

//TODO, merge 2 regex into one
std::string normalized_date(std::string text){
    std::vector<std::string> date_delim= {"/"};
    boost::regex re_date ("(\\d+)/(\\d+)/(\\d+)");   // matches date
    boost::cmatch matches;
    while (boost::regex_search (text.c_str(), matches,re_date)) {
        std::string day_str(matches[1].first, matches[1].second);
        std::string month_str(matches[2].first, matches[2].second);
        std::string year_str(matches[3].first, matches[3].second);
        //text.replace(matches.position(3) - 1, matches.length(3) + 1, conv_num2year(year_str) + " , "); // back 1 character to replace "/"
        text.replace(matches.position(3) - 1, matches.length(3) + 1, " năm " + conv_num2string(year_str) + " "); // back 1 character to replace "/"
        text.replace(matches.position(2) - 1, matches.length(2) + 1, conv_num2month(month_str));
        text.replace(matches.position(1), matches.length(1), conv_num2day(day_str));
    }

    boost::regex re_month_year ("(\\d+)/(\\d{4})");   // matches date
    while (boost::regex_search (text.c_str(), matches,re_month_year)) {
        std::string month_str(matches[1].first, matches[1].second);
        std::string year_str(matches[2].first, matches[2].second);
        text.replace(matches.position(2) - 1, matches.length(2) + 1, " năm " + conv_num2string(year_str) + " "); // back 1 character to replace "/"
        text.replace(matches.position(1), matches.length(1), conv_num2month(month_str));
    }

    boost::regex re_day_month ("(\\d+)/(\\d{2})");   // matches date
    while (boost::regex_search (text.c_str(), matches,re_day_month)) {
        std::string day_str(matches[1].first, matches[1].second);
        std::string month_str(matches[2].first, matches[2].second);
        text.replace(matches.position(2) - 1, matches.length(2) + 1, conv_num2month(month_str) + " , ");
        text.replace(matches.position(1), matches.length(1), conv_num2day(day_str));
    }

    boost::regex re_day_month_2 ("(\\d+)/(\\d{1})");   // matches date
    while (boost::regex_search (text.c_str(), matches,re_day_month_2)) {
        std::string day_str(matches[1].first, matches[1].second);
        std::string month_str(matches[2].first, matches[2].second);
        text.replace(matches.position(2) - 1, matches.length(2) + 1, conv_num2month(month_str) + " , ");
        text.replace(matches.position(1), matches.length(1), conv_num2day(day_str));
    }


    ReplaceStringInPlace(text, "  ", " ");
    ReplaceStringInPlace(text, "ngày ngày", "ngày");
    ReplaceStringInPlace(text, "tháng tháng", "tháng");
    return text;
}

std::string normalized_number(std::string text){
    boost::regex re_num ("(\\d+)");   // matches date
    boost::regex re_num_phay ("(\\d+),(\\d+)");   // matches date
    boost::cmatch matches;

    while (boost::regex_search (text.c_str(), matches,re_num_phay)) {
        std::string num1(matches[1].first, matches[1].second);
        std::string num2(matches[2].first, matches[2].second);
        text.replace(matches.position(2) - 1, matches.length(2) + 1, " phẩy " + conv_num2string(num2) + " "); // back 1 character to replace "/"
        text.replace(matches.position(1), matches.length(1), conv_num2string(num1));
    }

    while (boost::regex_search (text.c_str(), matches,re_num)) {
        std::string num_str(matches[1].first, matches[1].second);
        text.replace(matches.position(1), matches.length(1), conv_num2string(num_str));
    }
    return text;
}

std::string normalized_time(std::string text){
    boost::regex hour ("(\\d+)h");   // e.g. 2h
    boost::regex minute ("(\\d+)p");   // e.g. 20p
    boost::regex hour_minute ("(\\d+):(\\d+)");   // e.g. 10:20

    boost::cmatch matches;

    while (boost::regex_search (text.c_str(), matches,hour_minute)) {
        std::string num1(matches[1].first, matches[1].second);
        std::string num2(matches[2].first, matches[2].second);
        text.replace(matches.position(2) - 1, matches.length(2) + 1, conv_num2string(num2) + " phút "); // back 1 character to replace "/"
        text.replace(matches.position(1), matches.length(1), conv_num2string(num1) + " giờ ");
    }

    while (boost::regex_search (text.c_str(), matches, hour)) {
        std::string num1(matches[1].first, matches[1].second);
        // we only match the number, so we need to add 1 to matches.length() to replace `h` or `p`
        text.replace(matches.position(1), matches.length(1) + 1, conv_num2string(num1) + " giờ ");
    }

    while (boost::regex_search (text.c_str(), matches, minute)) {
        std::string num1(matches[1].first, matches[1].second);
        // we only match the number, so we need to add 1 to matches.length() to replace `h` or `p`
        text.replace(matches.position(1), matches.length(1) + 1, conv_num2string(num1) + " phút ");
    }

    return text;
}

std::string normalized_unit(std::string text){
    boost::regex meter ("\\d+(m)");   // e.g. 2m
    boost::regex massive_kg ("\\d+(kg)");   // e.g. 2kg
    boost::regex massive_g ("\\d+(g)");   // e.g. 2g
    boost::regex massive_g2 ("\\d+(gram)");   // e.g. 2g

    boost::cmatch matches;

    while (boost::regex_search (text.c_str(), matches, meter)) {
        std::string num1(matches[1].first, matches[1].second);
        text.replace(matches.position(1), matches.length(1) + 1, " mét ");
    }

    while (boost::regex_search (text.c_str(), matches, massive_kg)) {
        std::string num1(matches[1].first, matches[1].second);
        text.replace(matches.position(1), matches.length(1) + 1,  " ki lô gam ");
    }

    while (boost::regex_search (text.c_str(), matches, massive_g)) {
        std::string num1(matches[1].first, matches[1].second);
        text.replace(matches.position(1), matches.length(1) + 1, " gam ");
    }

    while (boost::regex_search (text.c_str(), matches, massive_g2)) {
        std::string num1(matches[1].first, matches[1].second);
        text.replace(matches.position(1), matches.length(1) + 1,  " gam ");
    }


    return text;
}
