/*
 * vita.h
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef VITA_H
#define VITA_H

#include <crfsuite.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <istream>
#include <OptionParser.h>
#include <text2feat.h>
#include <dictionary.h>
#include <vncommon.h>

std::string segment(CRFSuite::Tagger *, const std::string text);
std::string pos_tag(CRFSuite::Tagger *, const std::string text);
std::string chunk_tag(CRFSuite::Tagger *, const std::string text, const std::string pos);

#endif /* !VITA_H */
