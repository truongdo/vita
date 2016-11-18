/*
 * text_util.h
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef TEXT_UTIL_H
#define TEXT_UTIL_H

#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <boost/regex.hpp>
#include <iostream>

void SplitStringToVector(const std::string &full, const char *delim,
        bool omit_empty_strings,
        std::vector<std::string> *out);

void SplitStringToVector(const std::string &full,
        std::vector<std::string> *out);

void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace);

void clean(std::string &text);

std::string normalized_date(std::string text);

std::string normalized_number(std::string text);

std::string normalized_time(std::string text);

std::string normalized_unit(std::string text);
#endif /* !TEXT_UTIL_H */
