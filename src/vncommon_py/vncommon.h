/*
 * vncommon.h
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef VNCOMMON_H
#define VNCOMMON_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
#include <ctype.h>
#include <fstream>
#include <cstdlib>
#include "text_util.h"

struct _VNCommonLabelWord;
struct _VNCommonLabelPhonology;
struct _VNCommonLabelPhoneme;
struct _VNCommonLabelChunk;
struct _VNCommonLabel;

typedef struct _VNCommonLabelPhoneme{
    std::string phoneme;
    std::shared_ptr<_VNCommonLabelPhoneme> prev;
    std::shared_ptr<_VNCommonLabelPhoneme> next;
    std::shared_ptr<_VNCommonLabelPhonology> up;
} VNCommonLabelPhoneme;

typedef struct _VNCommonLabelPhonology{
    std::string text;
    std::vector<std::string> pron;
    std::string pos;
    std::string tone;
    std::shared_ptr<_VNCommonLabelPhonology> prev;
    std::shared_ptr<_VNCommonLabelPhonology> next;
    std::shared_ptr<_VNCommonLabelWord> up;
} VNCommonLabelPhonology;

typedef struct _VNCommonLabelWord {
    std::string text;
    std::vector<std::shared_ptr<VNCommonLabelPhonology> > phonology_list;
    std::string pos;
    std::string chunk;
    std::string tone;
    int wpos; // word position in the utterance
    bool iscompound;
    std::shared_ptr<_VNCommonLabelChunk> up;
    std::shared_ptr<_VNCommonLabelWord> prev;
    std::shared_ptr<_VNCommonLabelWord> next;
} VNCommonLabelWord;

typedef struct _VNCommonLabelChunk {
    std::string text;
    std::vector<std::shared_ptr<VNCommonLabelWord> > word_list;
    std::shared_ptr<_VNCommonLabelChunk> prev;
    std::shared_ptr<_VNCommonLabelChunk> next;
    std::shared_ptr<_VNCommonLabel> up;
} VNCommonLabelChunk;

typedef struct _VNCommonLabel {
    int size;
    std::vector<std::string> feature;
    std::shared_ptr<VNCommonLabelWord> word_head;
    std::shared_ptr<VNCommonLabelWord> word_tail;
    std::shared_ptr<VNCommonLabelPhonology> phonology_head;
    std::shared_ptr<VNCommonLabelPhonology> phonology_tail;
    std::shared_ptr<VNCommonLabelChunk> chunk_head;
    std::shared_ptr<VNCommonLabelChunk> chunk_tail;
    std::shared_ptr<VNCommonLabelPhoneme> phoneme_head;
    std::shared_ptr<VNCommonLabelPhoneme> phoneme_tail;
    bool short_pause_flag;
} VNCommonLabel;

void VNCommonLabel_free(std::shared_ptr<VNCommonLabel> label);
std::vector<std::string> VNCommonLabel_print(std::shared_ptr<_VNCommonLabel> label);
void VNCommonLabel_print(std::shared_ptr<VNCommonLabel> label, std::string myfile);
void VNCommonLabel_push_word(std::shared_ptr<_VNCommonLabel> label, std::string text, std::string pron, std::string pos, std::string chunk, std::string tone);
void _VNCommonLabel_push_word(std::shared_ptr<VNCommonLabel> label, std::shared_ptr<VNCommonLabelChunk>, std::string text, std::string pron, std::string pos, std::string tone);
void _VNCommonLabel_push_phonology(std::shared_ptr<VNCommonLabel> label, std::shared_ptr<VNCommonLabelWord> label_word, std::string text, std::string pron, std::string tone);
const char* P1(std::shared_ptr<VNCommonLabelPhoneme>);
const char* P2(std::shared_ptr<VNCommonLabelPhoneme>);
const char* P3(std::shared_ptr<VNCommonLabelPhoneme>);
const char* P4(std::shared_ptr<VNCommonLabelPhoneme>);
const char* P5(std::shared_ptr<VNCommonLabelPhoneme>);
int P6(std::shared_ptr<VNCommonLabelPhoneme>);
int P7(std::shared_ptr<VNCommonLabelPhoneme>);
const char* TONE(std::shared_ptr<VNCommonLabelPhonology> );
const char* POS_TAG(std::shared_ptr<VNCommonLabelWord> );
int COUNT_PHONEME_IN_PHONOLOGY(std::shared_ptr<VNCommonLabelPhonology> );
int COUNT_PHONEME_IN_WORD(std::shared_ptr<VNCommonLabelWord> );
int COUNT_PHONEME_IN_PHRASE(std::shared_ptr<VNCommonLabelChunk> );
int COUNT_PHONEME_IN_SENT(std::shared_ptr<VNCommonLabel> );

int COUNT_WORD_IN_PHRASE(std::shared_ptr<VNCommonLabelChunk> );
int COUNT_WORD_IN_SENT(std::shared_ptr<VNCommonLabel> );

int COUNT_PHRASE_IN_SENT(std::shared_ptr<VNCommonLabel> );

int COUNT_VOWEL(std::shared_ptr<VNCommonLabelPhonology> );
int PHONOLOGY_POS_IN_WORD_FORW(std::shared_ptr<VNCommonLabelPhonology>);
int PHRASE_POS_IN_SENT_FORW(std::shared_ptr<VNCommonLabelChunk>);
int PHRASE_POS_IN_SENT_BACK(std::shared_ptr<VNCommonLabelChunk>);
int PHONOLOGY_POS_IN_WORD_BACK(std::shared_ptr<VNCommonLabelPhonology>);
int PHONOLOGY_POS_IN_PHRASE_FORW(std::shared_ptr<VNCommonLabelPhonology>);
int PHONOLOGY_POS_IN_PHRASE_BACK(std::shared_ptr<VNCommonLabelPhonology>);
#endif /* !VNCOMMON_H */
