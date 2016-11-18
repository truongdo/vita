/*
 * vita.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include "vita.h"

using optparse::OptionParser;

void ana(std::string text, CRFSuite::Tagger &vn_segment,
        CRFSuite::Tagger &vn_pos, CRFSuite::Tagger &vn_chunk, Dictionary &dict, std::vector<std::string> &wlist,
        std::vector<std::string> &w_tag_list,
        std::vector<std::string> &w_chunk_list,
        std::vector<std::string> &tone_list, std::vector<std::string> &phone_list, bool out_tone=false){
    std::string sent = segment(&vn_segment, text);
    std::string pos = pos_tag(&vn_pos, sent);
    std::string chunk = chunk_tag(&vn_chunk, sent, pos);
    const char* delim = " ";

    SplitStringToVector(sent, delim, true, &wlist);
    SplitStringToVector(pos, delim, true, &w_tag_list);
    SplitStringToVector(chunk, delim, true, &w_chunk_list);

    for (std::size_t wid = 0; wid != wlist.size(); ++wid){
        tone_list.push_back("");
        phone_list.push_back("");
        dict.GetPhoneme(wlist[wid], &tone_list[wid], &phone_list[wid], out_tone);
    }
}

int main(int argc, char *argv[]){
    OptionParser parser = OptionParser().description("OpenVTalk text analysis");
    parser.add_option("-m", "--model_dir").dest("model_dir").help("Models directories").metavar("STRING");
    parser.add_option("-v", "--verbose").action("store_true").dest("verbose").set_default("0").help("Verbose");
    parser.add_option("-n", "--normalized").action("store_true").dest("norm").set_default("0").help("Normalizing text (i.e. date, number) before perform analysis");
    parser.add_option("-z", "--output-tone").action("store_true").dest("out_tone").set_default("0").help("Output pronounciations with tone (i.e. cais cs as is)");

    optparse::Values options = parser.parse_args(argc, argv);
    std::vector<std::string> args = parser.args();

    CRFSuite::Tagger vn_segment, vn_pos, vn_chunk;
    Dictionary dict;

    // Load segment model
    vn_segment.open(options["model_dir"] + "/word_segment.model");
    vn_pos.open(options["model_dir"]+"/word_pos.model");
    vn_chunk.open(options["model_dir"]+"/word_chunk.model");
    dict.LoadDict(options["model_dir"]+"/vn.dict");
    std::istream *in;
    std::ifstream ifn;

    if (!args.empty()){
        ifn.open(args[0]);
        in=&ifn;
    }
    else
        in=&std::cin;

    std::string line;
    while(getline(*in, line)){
        clean(line);
        if (options.get("norm")){
            normalized_date(line);
            normalized_number(line);
        }
        std::vector<std::string> wlist, w_tag, w_chunk, tone_list, p_list;
        ana(line, vn_segment, vn_pos, vn_chunk, dict, wlist, w_tag, w_chunk, tone_list, p_list, options.get("out_tone"));
        for (std::size_t wid = 0; wid != wlist.size(); ++wid){
            std::cout << wlist[wid] << "\t" << w_tag[wid] << "," << w_chunk[wid] << ","
                << tone_list[wid] << "," << p_list[wid] << std::endl;
        }
        std::cout << std::endl;
    }
}

std::string segment(CRFSuite::Tagger *vn_segment, const std::string text){
    CRFSuite::ItemSequence itemseq = text2feat_segment(vn_segment, text);
    CRFSuite::StringList result = vn_segment->tag(itemseq);
    std::vector<std::string>::iterator res;
    std::stringstream ss;
    std::vector<std::string> wlist;
    const char* delim = " ";
    SplitStringToVector(text, delim, true, &wlist);

    for(size_t i = 0; i < result.size(); ++i)
    {
        if(i != 0 ){
            if(result[i] == "I_W")
                ss << "_";
            else
                ss << " ";
        }
      ss << wlist[i];
    }
    return ss.str();
}

std::string pos_tag(CRFSuite::Tagger *vn_pos, const std::string text){
    CRFSuite::ItemSequence itemseq = text2feat_pos(vn_pos, text);
    CRFSuite::StringList result = vn_pos->tag(itemseq);
    std::vector<std::string>::iterator res;
    std::stringstream ss;
    std::vector<std::string> wlist;
    const char* delim = " ";
    SplitStringToVector(text, delim, true, &wlist);

    for(size_t i = 0; i < result.size(); ++i)
    {
        if(i != 0 ){
            ss << " ";
        }
      ss << result[i];
    }
    return ss.str();
}

std::string chunk_tag(CRFSuite::Tagger *vn_chunk, const std::string text, const std::string pos){
    CRFSuite::ItemSequence itemseq = text2feat_chunk(vn_chunk, text, pos);
    CRFSuite::StringList result = vn_chunk->tag(itemseq);
    std::vector<std::string>::iterator res;
    std::stringstream ss;
    std::vector<std::string> wlist;
    const char* delim = " ";
    SplitStringToVector(text, delim, true, &wlist);

    for(size_t i = 0; i < result.size(); ++i)
    {
        if(i != 0 ){
            ss << " ";
        }
      ss << result[i];
    }
    return ss.str();
}
