/*
 * vn_segment.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include "vtalk_ana.h"
#include "vncommon.h"
#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.hpp */

#include <cassert>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

using optparse::OptionParser;

class GenFullContext : public xmlrpc_c::method {
public:
    GenFullContext() {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
        this->_signature = "i:ii";  // method's arguments, result are integers
        this->_help = "This method generate fullcontext label";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {

        int const addend(paramList.getString(0));
        paramList.verifyEnd(1);

        *retvalP = xmlrpc_c::value_string(addend + adder);
    }
};

void ana(std::string text, CRFSuite::Tagger &vn_segment,
        CRFSuite::Tagger &vn_pos, Dictionary &dict, std::vector<std::string> &wlist,
        std::vector<std::string> &w_tag_list,
        std::vector<std::string> &tone_list, std::vector<std::string> &phone_list){
    std::string sent = segment(&vn_segment, text);
    std::string pos = pos_tag(&vn_pos, sent);
    const char* delim = " ";

    SplitStringToVector(sent, delim, true, &wlist);
    SplitStringToVector(pos, delim, true, &w_tag_list);

    for (std::size_t wid = 0; wid != wlist.size(); ++wid){
        tone_list.push_back("");
        phone_list.push_back("");
        dict.GetPhoneme(wlist[wid], &tone_list[wid], &phone_list[wid]);
    }
}

int main(int argc, char *argv[]){
    OptionParser parser = OptionParser().description("OpenVTalk text analysis");
    parser.add_option("-m", "--model_dir").dest("model_dir").help("Models directories").metavar("STRING");
    parser.add_option("-v", "--verbose").action("store_false").dest("verbose").set_default("1").help("Verbose");
    optparse::Values options = parser.parse_args(argc, argv);
    std::vector<std::string> args = parser.args();

    // Load segment model
    CRFSuite::Tagger vn_segment, vn_pos;
    Dictionary dict;

    vn_segment.open(options["model_dir"] + "/word_segment.model");
    vn_pos.open(options["model_dir"]+"/word_pos.model");
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
        clean(line); // clean text, delete punctuation
        normalized_date(line);
        normalized_number(line);
        std::shared_ptr<VNCommonLabel> label(new VNCommonLabel());
        VNCommonLabel_push_word(label, "sil", "sil", "xx", "xx" );
        std::vector<std::string> wlist, w_tag, tone_list, p_list;
        ana(line, vn_segment, vn_pos, dict, wlist, w_tag, tone_list, p_list);
        for (std::size_t wid = 0; wid != wlist.size(); ++wid){
            //std::cout << wlist[wid] << "\t" << w_tag[wid] << ","
                //<< tone_list[wid] << "," << p_list[wid] << std::endl;
            VNCommonLabel_push_word(label, wlist[wid], p_list[wid], w_tag[wid], tone_list[wid] );
        }
        VNCommonLabel_push_word(label, "sil", "sil", "xx", "xx" );
        VNCommonLabel_print(label);
    }
}


std::string segment(CRFSuite::Tagger *vn_segment, const std::string text){
    CRFSuite::ItemSequence itemseq = text2feat(vn_segment, text);
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
    CRFSuite::ItemSequence itemseq = text2feat(vn_pos, text);
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
