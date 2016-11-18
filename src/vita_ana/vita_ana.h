/*
 * vn_segment.h
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef VITA_ANA_H
#define VITA_ANA_H

#include <crfsuite.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <istream>
#include <OptionParser.h>
#include <text2feat.h>
#include <dictionary.h>
#include <text_util.h>

std::string segment(CRFSuite::Tagger *, const std::string text);
std::string pos_tag(CRFSuite::Tagger *, const std::string text);
std::string chunk_tag(CRFSuite::Tagger *vn_chunk, const std::string text, const std::string pos);
#endif /* !VITA_ANA_H */
