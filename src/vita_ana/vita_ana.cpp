/*
 * vn_segment.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include "vita_ana.h"
#include "vncommon.h"
#include "sstream"
#include "iterator"
using optparse::OptionParser;

void ana(std::string text, CRFSuite::Tagger &vn_segment,
        CRFSuite::Tagger &vn_pos, CRFSuite::Tagger &vn_chunk, Dictionary &dict, std::vector<std::string> &wlist,
        std::vector<std::string> &w_tag_list,
        std::vector<std::string> &w_chunk_list,
        std::vector<std::string> &tone_list, std::vector<std::string> &phone_list){
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
        dict.GetPhoneme(wlist[wid], &tone_list[wid], &phone_list[wid], false);
    }
}

std::string join(const std::vector<std::string>& vec, const char* delim)
{
  std::stringstream res;
     copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(res, delim));
     return res.str();
}

int main(int argc, char *argv[]){
    OptionParser parser = OptionParser().description("OpenVTalk text analysis");
    parser.add_option("-m", "--model_dir").dest("model_dir").help("Models directories").metavar("STRING");
    parser.add_option("-v", "--verbose").action("store_false").dest("verbose").set_default("1").help("Verbose");
    optparse::Values options = parser.parse_args(argc, argv);
    std::vector<std::string> args = parser.args();

    // Load segment model
    CRFSuite::Tagger vn_segment, vn_pos, vn_chunk;
    Dictionary dict;

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
      std::string filename = "";
      if (line.find("|") != std::string::npos){
        std::vector<std::string> parts;
        const char* delim = "|";
        SplitStringToVector(line, delim, true, &parts);
        filename = parts[0];
        parts.erase(parts.begin());
        line = join(parts, " ");
      }
        clean(line); // clean text, delete punctuation
        normalized_date(line);
        normalized_number(line);
        std::shared_ptr<VNCommonLabel> label(new VNCommonLabel());
        VNCommonLabel_push_word(label, "sil", "sil", "xx", "xx", "xx" );
        std::vector<std::string> wlist, w_tag, w_chunk, tone_list, p_list;
        ana(line, vn_segment, vn_pos, vn_chunk, dict, wlist, w_tag, w_chunk, tone_list, p_list);
        for (std::size_t wid = 0; wid != wlist.size(); ++wid){
            VNCommonLabel_push_word(label, wlist[wid], p_list[wid], w_tag[wid], w_chunk[wid], tone_list[wid] );
        }
        VNCommonLabel_push_word(label, "sil", "sil", "xx", "xx", "xx" );
      if (filename != ""){
        std::cout << "Process: " << filename << std::endl;
        VNCommonLabel_print(label, filename);
      }else{
        VNCommonLabel_print(label);
      }
        //std::cout << std::endl;
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
