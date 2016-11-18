/*
 * text2feat.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include <text2feat.h>
#include <map>


inline bool is_number(const std::string s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

inline bool is_title(const std::string s)
{
    // TODO compared with "Đ"
    if (isupper(s[0]))
        return true;
    else
        return false;
}

CRFSuite::ItemSequence text2feat_pos(CRFSuite::Tagger *tagger, const std::string text){
    /* Split string into word lists */
    CRFSuite::ItemSequence itemseq;

    std::vector<std::string> wlist;
    const char* delim = " ";
    std::string feat_delim = "|";
    SplitStringToVector(text, delim, true, &wlist);
    int wlist_size = static_cast<int>(wlist.size());
    //

    CRFSuite::StringList labels = tagger->labels();
    std::map<std::string, std::vector<int> > feature_map;
    feature_map["w[num]"] = {0}; feature_map["w[cap]"] = {0};
    feature_map["w[0]"] = {0}; feature_map["w[-1]"] = {-1};
    feature_map["w[-2]"] = {-2}; feature_map["w[1]"] = {1};
    feature_map["w[2]"] = {2}; feature_map["w[-2]|w[-1]"] = {-2, -1};
    feature_map["w[-1]|w[0]"] = {-1, 0}; feature_map["w[0]|w[1]"] = {0, 1};
    feature_map["w[1]|w[2]"] = {1, 2}; feature_map["w[-2]|w[-1]|w[0]"] = {-2, -1, 0};
    feature_map["w[-1]|w[0]|w[1]"] = {-1, 0, 1}; feature_map["w[0]|w[1]|w[2]"] = {0, 1, 2};
    feature_map["w[-2]|w[-1]|w[0]|w[1]"] = {-2, -1, 0, 1}; feature_map["w[-1]|w[0]|w[1]|w[2]"] = {-1, 0, 1, 2};
    feature_map["w[-2]|w[-1]|w[0]|w[1]|w[2]"] = {-2, -1, 0, 1, 2}; feature_map["w[0]|w[-1]"] = {0, -1};
    feature_map["w[0]|w[-2]"] = {0, -2}; feature_map["w[0]|w[-3]"] = {0, -3};
    feature_map["w[0]|w[-4]"] = {0, -4}; feature_map["w[0]|w[-5]"] = {0, -5};
    feature_map["w[0]|w[-6]"] = {0, -6}; feature_map["w[0]|w[-7]"] = {0, -7};
    feature_map["w[0]|w[-8]"] = {0, -8}; feature_map["w[0]|w[-9]"] = {0, -9};
    feature_map["w[0]|w[1]"] = {0, 1}; feature_map["w[0]|w[2]"] = {0, 2};
    feature_map["w[0]|w[3]"] = {0, 3}; feature_map["w[0]|w[4]"] = {0, 4};
    feature_map["w[0]|w[5]"] = {0, 5}; feature_map["w[0]|w[6]"] = {0, 6};
    feature_map["w[0]|w[7]"] = {0, 7}; feature_map["w[0]|w[8]"] = {0, 8};
    feature_map["w[0]|w[9]"] = {0, 9};
    std::map<std::string, std::vector<int> >::iterator feat_item;

    for (std::size_t wid=0, max=wlist.size(); wid != max; ++wid){
        CRFSuite::Item item;
        for (feat_item = feature_map.begin(); feat_item != feature_map.end(); ++feat_item){
            std::vector<int>::iterator feat_idx;
            std::vector<std::string> attr;
            bool feat_ok = true;
            for (feat_idx = feat_item->second.begin(); feat_idx != feat_item->second.end(); ++feat_idx){
                int w_feat_id = wid + *feat_idx;
                if (feat_item->first == "w[num]"){
                    if (is_number(wlist[w_feat_id]))
                        attr.push_back("True");
                    else
                        attr.push_back("False");
                    continue;
                }

                if (feat_item->first == "w[cap]"){
                    if (is_title(wlist[w_feat_id]))
                        attr.push_back("True");
                    else
                        attr.push_back("False");
                    continue;
                }

                if (w_feat_id >= 0 && w_feat_id < wlist_size){
                    attr.push_back(wlist[w_feat_id]);
                }else{
                    feat_ok = false;
                    break;
                }
            }

            // If ok
            if (feat_ok){
                std::stringstream ss;
                for(size_t i = 0; i < attr.size(); ++i)
                {
                  if(i != 0)
                    ss << "|";
                  ss << attr[i];
                }
                CRFSuite::Attribute crf_attr(feat_item->first + "=" + ss.str());
                item.push_back(crf_attr);
            }
        }
        itemseq.push_back(item);
    }

    return itemseq;
}

CRFSuite::ItemSequence text2feat_segment(CRFSuite::Tagger *tagger, const std::string text){
    /* Split string into word lists */
    CRFSuite::ItemSequence itemseq;

    std::vector<std::string> wlist;
    const char* delim = " ";
    std::string feat_delim = "|";
    SplitStringToVector(text, delim, true, &wlist);
    int wlist_size = static_cast<int>(wlist.size());
    //

    CRFSuite::StringList labels = tagger->labels();
    std::map<std::string, std::vector<int> > feature_map;
    feature_map["w[0]"] = {0}; feature_map["w[-1]"] = {-1};
    feature_map["w[-2]"] = {-2}; feature_map["w[1]"] = {1};
    feature_map["w[2]"] = {2}; feature_map["-2]|[-1"] = {-2, -1};
    feature_map["w[-1]|[0]"] = {-1, 0}; feature_map["w[0]|[1]"] = {0, 1};
    feature_map["w[1]|[2]"] = {1, 2}; feature_map["w[-2]|[-1]|[0]"] = {-2, -1, 0};
    feature_map["w[-1]|w[0]|w[1]"] = {-1, 0, 1}; feature_map["w[0]|w[1]|w[2]"] = {0, 1, 2};
    feature_map["w[-2]|w[-1]|w[0]|w[1]"] = {-2, -1, 0, 1}; feature_map["w[-1]|w[0]|w[1]|w[2]"] = {-1, 0, 1, 2};
    feature_map["w[-2]|w[-1]|w[0]|w[1]|w[2]"] = {-2, -1, 0, 1, 2}; feature_map["w[0]|w[-1]"] = {0, -1};
    feature_map["w[0]|w[-2]"] = {0, -2}; feature_map["w[0]|w[-3]"] = {0, -3};
    feature_map["w[0]|w[-4]"] = {0, -4}; feature_map["w[0]|w[-5]"] = {0, -5};
    feature_map["w[0]|w[-6]"] = {0, -6}; feature_map["w[0]|w[-7]"] = {0, -7};
    feature_map["w[0]|w[-8]"] = {0, -8}; feature_map["w[0]|w[-9]"] = {0, -9};
    feature_map["w[0]|w[1]"] = {0, 1}; feature_map["w[0]|w[2]"] = {0, 2};
    feature_map["w[0]|w[3]"] = {0, 3}; feature_map["w[0]|w[4]"] = {0, 4};
    feature_map["w[0]|w[5]"] = {0, 5}; feature_map["w[0]|w[6]"] = {0, 6};
    feature_map["w[0]|w[7]"] = {0, 7}; feature_map["w[0]|w[8]"] = {0, 8};
    feature_map["w[0]|w[9]"] = {0, 9};
    std::map<std::string, std::vector<int> >::iterator feat_item;

    for (std::size_t wid=0, max=wlist.size(); wid != max; ++wid){
        CRFSuite::Item item;
        for (feat_item = feature_map.begin(); feat_item != feature_map.end(); ++feat_item){
            std::vector<int>::iterator feat_idx;
            std::vector<std::string> attr;
            bool feat_ok = true;
            for (feat_idx = feat_item->second.begin(); feat_idx != feat_item->second.end(); ++feat_idx){
                int w_feat_id = wid + *feat_idx;
                if (w_feat_id >= 0 && w_feat_id < wlist_size){
                    attr.push_back(wlist[w_feat_id]);
                }else{
                    feat_ok = false;
                    break;
                }
            }

            // If ok
            if (feat_ok){
                std::stringstream ss;
                for(size_t i = 0; i < attr.size(); ++i)
                {
                  if(i != 0)
                    ss << "|";
                  ss << attr[i];
                }
                CRFSuite::Attribute crf_attr(feat_item->first + "=" + ss.str());
                item.push_back(crf_attr);
            }
        }
        itemseq.push_back(item);
    }

    return itemseq;
}

CRFSuite::ItemSequence text2feat_chunk(CRFSuite::Tagger *tagger, const std::string text, const std::string pos){
    /* Split string into word lists */
    CRFSuite::ItemSequence itemseq;

    std::vector<std::string> wlist;
    const char* delim = " ";
    std::string feat_delim = "|";
    SplitStringToVector(text, delim, true, &wlist);
    int wlist_size = static_cast<int>(wlist.size());

    std::vector<std::string> poslist;
    SplitStringToVector(pos, delim, true, &poslist);
    int poslist_size = static_cast<int>(poslist.size());
    //

    assert(wlist_size == poslist_size);

    CRFSuite::StringList labels = tagger->labels();
    std::map<std::string, std::vector<int> > feature_map;
    feature_map["w[-2]"] = {-2};
    feature_map["w[-1]"] = {-1};
    feature_map["w[0]"] = {0};
    feature_map["w[1]"] = {1};
    feature_map["w[2]"] = {2};
    feature_map["w[-1]|w[0]"] = {-1, 0};
    feature_map["w[0]|w[1]"] = {0, 1};
    feature_map["pos[-2]"] = {-2};
    feature_map["pos[-1]"] = {-1};
    feature_map["pos[0]"] = {0};
    feature_map["pos[1]"] = {1};
    feature_map["pos[2]"] = {2};
    feature_map["pos[-2]|pos[-1]"] = {-2, -1};
    feature_map["pos[-1]|pos[0]"] = {-1, 0};
    feature_map["pos[0]|pos[1]"] = {0, 1};
    feature_map["pos[1]|pos[2]"] = {1, 2};
    feature_map["pos[-2]|pos[-1]|pos[0]"] = {-2, -1, 0};
    feature_map["pos[-1]|pos[0]|pos[1]"] = {-1, 0, 1};
    feature_map["pos[0]|pos[1]|pos[2]"] = {0, 1, 2};

    std::map<std::string, std::vector<int> >::iterator feat_item;

    for (std::size_t wid=0, max=wlist.size(); wid != max; ++wid){
        CRFSuite::Item item;
        for (feat_item = feature_map.begin(); feat_item != feature_map.end(); ++feat_item){
            std::vector<int>::iterator feat_idx;
            std::vector<std::string> attr;
            bool feat_ok = true;
            for (feat_idx = feat_item->second.begin(); feat_idx != feat_item->second.end(); ++feat_idx){
                int w_feat_id = wid + *feat_idx;

                if (w_feat_id >= 0 && w_feat_id < wlist_size){
                    if (feat_item->first.find("pos") != std::string::npos) {
                        attr.push_back(poslist[w_feat_id]);
                    }else{
                        attr.push_back(wlist[w_feat_id]);
                    }
                }else{
                    feat_ok = false;
                    break;
                }


            }

            // If ok
            if (feat_ok){
                std::stringstream ss;
                for(size_t i = 0; i < attr.size(); ++i)
                {
                  if(i != 0)
                    ss << "|";
                  ss << attr[i];
                }
                CRFSuite::Attribute crf_attr(feat_item->first + "=" + ss.str());
                item.push_back(crf_attr);
            }
        }
        itemseq.push_back(item);
    }

    return itemseq;
}
