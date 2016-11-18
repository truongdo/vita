/*
 * text2feat.h
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef TEXT2FEAT_H
#define TEXT2FEAT_H

#include <string>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <crfsuite.hpp>
#include <text_util.h>
#include <stdlib.h>
#include <assert.h>     /* assert */


std::vector<std::string > content_words = {"N", "V", "A", "R", "P"};

CRFSuite::ItemSequence text2feat_segment(CRFSuite::Tagger *, const std::string);
CRFSuite::ItemSequence text2feat_pos(CRFSuite::Tagger *, const std::string);
CRFSuite::ItemSequence text2feat_chunk(CRFSuite::Tagger *, const std::string, const std::string);
#endif /* !TEXT2FEAT_H */
