/*
 * vncommon.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include "vncommon.h"
#include "logging.cpp"

const char* wrap_str(std::string text){
    return text.c_str();
}


std::vector<std::string> vowel = {"a", "aa", "aw", "e", "ea", "ee", "i", "ie", "iz"
        "o", "oa", "oo", "ow", "u", "uo", "uw", "uz", "wa"};
void VNCommonLabel_initialize(VNCommonLabel *label){
    if (label == NULL){
        OpenVtalk_ERROR(__FILE__, __LINE__, "Label in null");
    }
    label->word_head = NULL;
    label->word_tail = NULL;
    label->phoneme_head = NULL;
    label->phoneme_tail = NULL;
    label->short_pause_flag = false;
}

void VNCommonLabel_free(std::shared_ptr<VNCommonLabel> label){
    label->word_head = NULL;
    label->word_tail = NULL;
    label->phoneme_head = NULL;
    label->phoneme_tail = NULL;
    label->short_pause_flag = false;
    label = NULL;
}

void _VNCommonLabel_push_phonology(std::shared_ptr<VNCommonLabel> label, std::shared_ptr<VNCommonLabelWord> label_word, std::string text,
        std::string pron, std::string tone){
    if (label == NULL){
        OpenVtalk_ERROR(__FILE__, __LINE__, "Label in null");
    }
    std::shared_ptr<VNCommonLabelPhonology> label_phonology(new VNCommonLabelPhonology());
    label_word->phonology_list.push_back(label_phonology);

    label_phonology->text = text;
    SplitStringToVector(pron, &label_phonology->pron);
    label_phonology->tone = tone;
    label_phonology->next = label_phonology->prev = NULL;
    label_phonology->up = label_word;

    if (label->phonology_tail == NULL){
        label->phonology_tail = label_phonology;
        label->phonology_head = label->phonology_tail;
    }else{
        label_phonology->prev = label->phonology_tail;
        label->phonology_tail->next = label_phonology;
        label->phonology_tail = label_phonology;
    }
    //[> Phoneme analysis <]
    for (size_t idx = 0; idx < label_phonology->pron.size(); idx++){
        std::shared_ptr<VNCommonLabelPhoneme> phone(new VNCommonLabelPhoneme());
        phone->phoneme = label_phonology->pron[idx];
        phone->next = phone->prev = NULL;

        phone->up = label_phonology;  // up point to phonology
        if (label->phoneme_tail == NULL){
            label->phoneme_tail = phone;
            label->phoneme_head = label->phoneme_tail;
        }else{
            phone->prev = label->phoneme_tail;
            label->phoneme_tail->next = phone;
            label->phoneme_tail = phone;
        }
    }
}

void _VNCommonLabel_push_word(std::shared_ptr<VNCommonLabel> label, std::shared_ptr<VNCommonLabelChunk> label_chunk, std::string text, std::string pron,
        std::string pos, std::string tone){

    if (ispunct(text.c_str()[0])){
        pron = "pau";
        pos = "xx";
        tone = "xx";
    }

    if (label == NULL){
        OpenVtalk_ERROR(__FILE__, __LINE__, "Label in null");
    }
    std::shared_ptr<VNCommonLabelWord> label_word(new VNCommonLabelWord());
    label_word->text = text;
    //SplitStringToVector(pron, &label_word->pron);
    label_word->pos = pos;
    label_word->tone = tone;
    label_word->next = label_word->prev = NULL;
    label_word->up = label_chunk;

    if (label->word_tail == NULL){
        label->word_tail = label_word;
        label->word_head = label->word_tail;
    }else{
        label_word->prev = label->word_tail;
        label->word_tail->next = label_word;
        label->word_tail = label_word;
    }

    // Push word into the current chunk
    label_chunk->word_list.push_back(label_word);
    if (text.find("_") != std::string::npos){
        std::vector<std::string> word_list, pron_list, tone_list;
        const char* delim = "_";
        SplitStringToVector(text, delim, true, &word_list);
        SplitStringToVector(pron, delim, true, &pron_list);
        SplitStringToVector(tone, delim, true, &tone_list);

        for (size_t idx = 0; idx < word_list.size(); idx++){
            _VNCommonLabel_push_phonology(label, label_word, word_list[idx], pron_list[idx], tone_list[idx]);
        }
    }else{
       _VNCommonLabel_push_phonology(label, label_word, text, pron, tone);
    }
}

void VNCommonLabel_push_word(std::shared_ptr<_VNCommonLabel> label, std::string text, std::string pron,
        std::string pos, std::string chunk, std::string tone){
    std::shared_ptr<VNCommonLabelChunk> label_chunk = NULL;
    if (chunk.find("B-") != std::string::npos || chunk.find("xx") != std::string::npos){
        label_chunk = std::shared_ptr<VNCommonLabelChunk> (new VNCommonLabelChunk());
        if (label->chunk_tail == NULL){
            label->chunk_tail = label_chunk;
            label->chunk_head = label->chunk_tail;
        }else{
            label_chunk->prev = label->chunk_tail;
            label->chunk_tail->next = label_chunk;
            label->chunk_tail = label_chunk;
        }
    }else{
        label_chunk = label->chunk_tail;
    }

    label_chunk->up = label;
    _VNCommonLabel_push_word(label, label_chunk, text, pron, pos, tone);

}

std::vector<std::string> VNCommonLabel_print(std::shared_ptr<_VNCommonLabel> label){
    std::shared_ptr<VNCommonLabelPhoneme> phone = NULL;
    std::vector<std::string> output;
    for (phone = label->phoneme_head; phone != NULL; phone = phone->next){
        char buffer [2000];
        sprintf(buffer, "%s^%s-%s+%s=%s@%d_%d\
/A:%s_%d\
/B:%s-%d\
@%d-%d\
&%d-%d\
/C:%s+%d\
/D:%s-%d\
/E:%s+%d\
/F:%s-%d\
/G:%d-%d\
/H:%d=%d\
@%d=%d\
/I:%d_%d\
/J:%d+%d-%d",
                P1(phone), P2(phone), P3(phone), P4(phone), P5(phone), P6(phone), P7(phone),
                TONE(phone->up->prev), COUNT_PHONEME_IN_PHONOLOGY(phone->up->prev),
                TONE(phone->up), COUNT_PHONEME_IN_PHONOLOGY(phone->up),
                PHONOLOGY_POS_IN_WORD_FORW(phone->up), PHONOLOGY_POS_IN_WORD_BACK(phone->up),
                PHONOLOGY_POS_IN_PHRASE_FORW(phone->up), PHONOLOGY_POS_IN_PHRASE_BACK(phone->up),
                TONE(phone->up->next), COUNT_PHONEME_IN_PHONOLOGY(phone->up->next),
                POS_TAG(phone->up->up->prev), COUNT_PHONEME_IN_WORD(phone->up->up->prev),
                POS_TAG(phone->up->up), COUNT_PHONEME_IN_WORD(phone->up->up),
                POS_TAG(phone->up->up->next), COUNT_PHONEME_IN_WORD(phone->up->up->next),
                COUNT_PHONEME_IN_PHRASE(phone->up->up->up->prev), COUNT_WORD_IN_PHRASE(phone->up->up->up->prev),
                COUNT_PHONEME_IN_PHRASE(phone->up->up->up), COUNT_WORD_IN_PHRASE(phone->up->up->up),
                PHRASE_POS_IN_SENT_FORW(phone->up->up->up), PHRASE_POS_IN_SENT_BACK(phone->up->up->up),
                COUNT_PHONEME_IN_PHRASE(phone->up->up->up->next), COUNT_WORD_IN_PHRASE(phone->up->up->up->next),
                COUNT_PHONEME_IN_SENT(phone->up->up->up->up), COUNT_WORD_IN_SENT(phone->up->up->up->up), COUNT_PHRASE_IN_SENT(phone->up->up->up->up)
                );
        /*
                        &%s-%s\
                        #%s-%s$%s-%s!%s-%s;%s-%s|%s/C:%s+%s+%s/D:%s-%s/E:%s+%s@\
                        %s+%s&%s+%s#%s+%s/F:%s-%s/G:%s-%s/H:%s=%s@%s=%s|%s/I:%s_%s/ \
                        J:%s+%s-%s",

        sprintf(buffer, "%s^%s-%s+%s=%s@%d_%d/A:%s_%s_%d/B:%s-%s-%d@%d/C:%s_%s_%d", P1(phone).c_str(),
                P2(phone).c_str(), P3(phone).c_str(), P4(phone).c_str(), P5(phone).c_str(), P6(phone), P7(phone),
                TONE(phone->up->prev).c_str(), POS_TAG(phone->up->up->prev).c_str(), COUNT_PHONEME(phone->up->prev),
                TONE(phone->up).c_str(), POS_TAG(phone->up->up).c_str(),COUNT_PHONEME(phone->up), COUNT_VOWEL(phone->up),
                TONE(phone->up->next).c_str(), POS_TAG(phone->up->up->next).c_str(),
                 COUNT_PHONEME(phone->up->next)
                );
                */

        //std::cout << buffer << "" << std::endl;
        std::string s(buffer);
        output.push_back(s);
    }
    return output;
}

void VNCommonLabel_print(std::shared_ptr<VNCommonLabel> label, std::string myfile){
    std::shared_ptr<VNCommonLabelPhoneme> phone = NULL;
    std::ofstream fout;
    fout.open(myfile);
    for (phone = label->phoneme_head; phone != NULL; phone = phone->next){
        char buffer [2000];
        sprintf(buffer, "%s^%s-%s+%s=%s@%d_%d\
/A:%s_%d\
/B:%s-%d\
@%d-%d\
&%d-%d\
/C:%s+%d\
/D:%s-%d\
/E:%s+%d\
/F:%s-%d\
/G:%d-%d\
/H:%d=%d\
@%d=%d\
/I:%d_%d\
/J:%d+%d-%d",
                P1(phone), P2(phone), P3(phone), P4(phone), P5(phone), P6(phone), P7(phone),
                TONE(phone->up->prev), COUNT_PHONEME_IN_PHONOLOGY(phone->up->prev),
                TONE(phone->up), COUNT_PHONEME_IN_PHONOLOGY(phone->up),
                PHONOLOGY_POS_IN_WORD_FORW(phone->up), PHONOLOGY_POS_IN_WORD_BACK(phone->up),
                PHONOLOGY_POS_IN_PHRASE_FORW(phone->up), PHONOLOGY_POS_IN_PHRASE_BACK(phone->up),
                TONE(phone->up->next), COUNT_PHONEME_IN_PHONOLOGY(phone->up->next),
                POS_TAG(phone->up->up->prev), COUNT_PHONEME_IN_WORD(phone->up->up->prev),
                POS_TAG(phone->up->up), COUNT_PHONEME_IN_WORD(phone->up->up),
                POS_TAG(phone->up->up->next), COUNT_PHONEME_IN_WORD(phone->up->up->next),
                COUNT_PHONEME_IN_PHRASE(phone->up->up->up->prev), COUNT_WORD_IN_PHRASE(phone->up->up->up->prev),
                COUNT_PHONEME_IN_PHRASE(phone->up->up->up), COUNT_WORD_IN_PHRASE(phone->up->up->up),
                PHRASE_POS_IN_SENT_FORW(phone->up->up->up), PHRASE_POS_IN_SENT_BACK(phone->up->up->up),
                COUNT_PHONEME_IN_PHRASE(phone->up->up->up->next), COUNT_WORD_IN_PHRASE(phone->up->up->up->next),
                COUNT_PHONEME_IN_SENT(phone->up->up->up->up), COUNT_WORD_IN_SENT(phone->up->up->up->up), COUNT_PHRASE_IN_SENT(phone->up->up->up->up)
                );
        /*
                        &%s-%s\
                        #%s-%s$%s-%s!%s-%s;%s-%s|%s/C:%s+%s+%s/D:%s-%s/E:%s+%s@\
                        %s+%s&%s+%s#%s+%s/F:%s-%s/G:%s-%s/H:%s=%s@%s=%s|%s/I:%s_%s/ \
                        J:%s+%s-%s",

        sprintf(buffer, "%s^%s-%s+%s=%s@%d_%d/A:%s_%s_%d/B:%s-%s-%d@%d/C:%s_%s_%d", P1(phone).c_str(),
                P2(phone).c_str(), P3(phone).c_str(), P4(phone).c_str(), P5(phone).c_str(), P6(phone), P7(phone),
                TONE(phone->up->prev).c_str(), POS_TAG(phone->up->up->prev).c_str(), COUNT_PHONEME(phone->up->prev),
                TONE(phone->up).c_str(), POS_TAG(phone->up->up).c_str(),COUNT_PHONEME(phone->up), COUNT_VOWEL(phone->up),
                TONE(phone->up->next).c_str(), POS_TAG(phone->up->up->next).c_str(),
                 COUNT_PHONEME(phone->up->next)
                );
                */

        fout << buffer << "" << std::endl;
    }
    fout.close();
}

int COUNT_PHONEME_IN_SENT(std::shared_ptr<VNCommonLabel> label){
    if (! label)
        return 0;

    std::shared_ptr<VNCommonLabelChunk> p_chunk = label->chunk_head;

    int count = 0;
    for (; p_chunk != label->chunk_tail; p_chunk = p_chunk->next){
        count += COUNT_PHONEME_IN_PHRASE(p_chunk);
    }
    return count + 1;
}


int COUNT_WORD_IN_SENT(std::shared_ptr<VNCommonLabel> label){
    if (! label)
        return 0;

    std::shared_ptr<VNCommonLabelChunk> p_chunk = label->chunk_head;

    int count = 0;
    for (; p_chunk != label->chunk_tail; p_chunk = p_chunk->next){
        count += COUNT_WORD_IN_PHRASE(p_chunk);
    }
    return count - 1;
}

int COUNT_PHRASE_IN_SENT(std::shared_ptr<VNCommonLabel> label){
    if (! label)
        return 0;

    std::shared_ptr<VNCommonLabelChunk> p_chunk = label->chunk_head;

    int count = 0;
    for (; p_chunk != label->chunk_tail; p_chunk = p_chunk->next){
        count += 1;
    }
    return count - 1;
}

int PHRASE_POS_IN_SENT_FORW(std::shared_ptr<VNCommonLabelChunk> curr_chunk){
    std::shared_ptr<VNCommonLabelChunk> p_chunk = curr_chunk->up->chunk_head;

    int idx = 0;
    for (; p_chunk != curr_chunk->up->chunk_tail; p_chunk = p_chunk->next){
        if (p_chunk == curr_chunk){
            break;
        }
        idx++;
    }
    return idx;
}

int PHRASE_POS_IN_SENT_BACK(std::shared_ptr<VNCommonLabelChunk> curr_chunk){
    std::shared_ptr<VNCommonLabelChunk> p_chunk = curr_chunk->up->chunk_tail;

    int idx = 0;
    for (; p_chunk != curr_chunk->up->chunk_head; p_chunk = p_chunk->prev){
        if (p_chunk == curr_chunk){
            break;
        }
        idx++;
    }
    return idx;
}

int PHONOLOGY_POS_IN_PHRASE_FORW(std::shared_ptr<VNCommonLabelPhonology> curr_phonology){
    /*
     * 1. We first find the word position in the current phrase
     * 2. find the phonology position  in the current word
     * -> phonology position in the current phrase
     */
    std::vector<std::shared_ptr<VNCommonLabelWord> >::iterator it = std::find(curr_phonology->up->up->word_list.begin(),
            curr_phonology->up->up->word_list.end(), curr_phonology->up);
    if (it != curr_phonology->up->up->word_list.end()){
        int word_pos = it - curr_phonology->up->up->word_list.begin();
        int phonology_pos = PHONOLOGY_POS_IN_WORD_FORW(curr_phonology);
        return word_pos + phonology_pos;
    }
    else
        OpenVtalk_ERROR(__FILE__, __LINE__, "can not find phone " + curr_phonology->text);
    return -1; // never reach, make the compiler happy
}

int PHONOLOGY_POS_IN_PHRASE_BACK(std::shared_ptr<VNCommonLabelPhonology> curr_phonology){
    /*
     * 1. We first find the word position in the current phrase
     * 2. find the phonology position  in the current word
     * -> phonology position in the current phrase
     */
    std::vector<std::shared_ptr<VNCommonLabelWord> >::iterator it = std::find(curr_phonology->up->up->word_list.begin(),
            curr_phonology->up->up->word_list.end(), curr_phonology->up);
    if (it != curr_phonology->up->up->word_list.end()){
        int word_pos = curr_phonology->up->up->word_list.end() - it - 1;
        int phonology_pos = PHONOLOGY_POS_IN_WORD_BACK(curr_phonology);
        return word_pos + phonology_pos;
    }
    else
        OpenVtalk_ERROR(__FILE__, __LINE__, "can not find phone " + curr_phonology->text);
    return -1; // never reach, make the compiler happy
}

int PHONOLOGY_POS_IN_WORD_FORW(std::shared_ptr<VNCommonLabelPhonology> curr_phonology){
    std::vector<std::shared_ptr<VNCommonLabelPhonology> >::iterator it = std::find(curr_phonology->up->phonology_list.begin(),
            curr_phonology->up->phonology_list.end(), curr_phonology);
    if (it != curr_phonology->up->phonology_list.end())
        return it - curr_phonology->up->phonology_list.begin();
    else
        OpenVtalk_ERROR(__FILE__, __LINE__, "can not find phone " + curr_phonology->text);
    return -1; // never reach, make the compiler happy
}

int PHONOLOGY_POS_IN_WORD_BACK(std::shared_ptr<VNCommonLabelPhonology> curr_phonology){
    std::vector<std::shared_ptr<VNCommonLabelPhonology> >::iterator it = std::find(curr_phonology->up->phonology_list.begin(),
            curr_phonology->up->phonology_list.end(), curr_phonology);
    if (it != curr_phonology->up->phonology_list.end())
        return curr_phonology->up->phonology_list.end() - it - 1;
    else
        OpenVtalk_ERROR(__FILE__, __LINE__, "can not find phone " + curr_phonology->text);
    return -1; // never reach, make the compiler happy
}

const char* P1(std::shared_ptr<VNCommonLabelPhoneme> currphone){
    if (currphone == NULL){
        OpenVtalk_ERROR(__FILE__, __LINE__, "Currentphone is null");
    }
    if (currphone->prev){
        if (currphone->prev->prev)
            return currphone->prev->prev->phoneme.c_str();
        else
            return "xx";
    }else
        return "xx";
}

const char* P2(std::shared_ptr<VNCommonLabelPhoneme> currphone){
    if (currphone == NULL){
        OpenVtalk_ERROR(__FILE__, __LINE__, "Currentphone is null");
    }
    if (currphone->prev){
        return currphone->prev->phoneme.c_str();
    }else
        return "xx";
}

const char* P3(std::shared_ptr<VNCommonLabelPhoneme> currphone){
    if (currphone == NULL){
        OpenVtalk_ERROR(__FILE__, __LINE__, "Currentphone is null");
    }
    return currphone->phoneme.c_str();
}

const char* P4(std::shared_ptr<VNCommonLabelPhoneme> currphone){
    if (currphone == NULL){
        OpenVtalk_ERROR(__FILE__, __LINE__, "Currentphone is null");
    }
    if (currphone->next)
        return currphone->next->phoneme.c_str();
    else
        return "xx";
}

const char* P5(std::shared_ptr<VNCommonLabelPhoneme> currphone){
    if (currphone == NULL){
        OpenVtalk_ERROR(__FILE__, __LINE__, "Currentphone is null");
    }
    if (currphone->next){
        if (currphone->next->next)
            return currphone->next->next->phoneme.c_str();
        else
            return "xx";
    }else
        return "xx";
}

int P6(std::shared_ptr<VNCommonLabelPhoneme> currphone){
    std::vector<std::string>::iterator it = std::find(currphone->up->pron.begin(),
            currphone->up->pron.end(), currphone->phoneme);
    if (it != currphone->up->pron.end())
        return it - currphone->up->pron.begin();
    else
        OpenVtalk_ERROR(__FILE__, __LINE__, "can not find phone " + currphone->phoneme);
    return -1; // never reach, make compile happy
}

int P7(std::shared_ptr<VNCommonLabelPhoneme> currphone){
    std::vector<std::string>::iterator it = std::find(currphone->up->pron.begin(),
            currphone->up->pron.end(), currphone->phoneme);
    if (it != currphone->up->pron.end())
        return currphone->up->pron.end() - it - 1;
    else
        OpenVtalk_ERROR(__FILE__, __LINE__, "can not find phone " + currphone->phoneme);
    return -1; // never reach, make compile happy
}

const char* TONE(std::shared_ptr<VNCommonLabelPhonology> phonology){
    if (phonology)
        return (phonology->tone == "") ? "xx": phonology->tone.c_str();
    else
        return "xx";
}

const char* POS_TAG(std::shared_ptr<VNCommonLabelWord> word){
    if (word)
        return (word->pos == "") ? "xx": word->pos.c_str();
    else
        return "xx";
}

// Number of phoneme in the current phonology
int COUNT_PHONEME_IN_PHONOLOGY(std::shared_ptr<VNCommonLabelPhonology> phonology){
    if (phonology)
        return phonology->pron.size();
    else
        return 0;
}

// Number of phoneme in the current word
int COUNT_PHONEME_IN_WORD(std::shared_ptr<VNCommonLabelWord> word){
    if (! word){
      return 0;
    }

    std::vector<std::shared_ptr<VNCommonLabelPhonology> >::iterator it = word->phonology_list.begin();

    int phone_count = 0;
    for ( ; it != word->phonology_list.end(); it++){
        phone_count += COUNT_PHONEME_IN_PHONOLOGY(*it);
    }

    return phone_count;
}

// Number of phoneme in the current phrase
int COUNT_PHONEME_IN_PHRASE(std::shared_ptr<VNCommonLabelChunk> phrase){
    if (! phrase){
      return 0;
    }

    std::vector<std::shared_ptr<VNCommonLabelWord> >::iterator it = phrase->word_list.begin();

    int phone_count = 0;
    for ( ; it != phrase->word_list.end(); it++){
        phone_count += COUNT_PHONEME_IN_WORD(*it);
    }

    return phone_count;
}

// Number of word in the current phrase
int COUNT_WORD_IN_PHRASE(std::shared_ptr<VNCommonLabelChunk> phrase){
    if (! phrase){
      return 0;
    }

    return phrase->word_list.size();
}

// Number of vowel in the current phonology
int COUNT_VOWEL(std::shared_ptr<VNCommonLabelPhonology> phonology){
    if (phonology){
        std::vector<std::string>::iterator it;
        int count = 0;
        for (size_t idx = 0; idx < phonology->pron.size(); idx++){
            std::vector<std::string>::iterator found = std::find(vowel.begin(), vowel.end(), phonology->pron[idx]);
            if (found != vowel.end()){
                count++;
            }
        }
        return count;
    }
    else
        return 0;
}
