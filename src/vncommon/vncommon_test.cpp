/*
 * vncommon_test.cpp
 * Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
 *
 * Distributed under terms of the MIT license.
 */

#include <vncommon.h>

int main(){
    std::shared_ptr<VNCommonLabel> label(new VNCommonLabel());
    //_VNCommonLabel_push_word(label, "how", "h o w", "N", "B-W", "2");
    VNCommonLabel_print(label);
    VNCommonLabel_free(label);
    //VNCommonLabel_push_word(label, "are", "a r e", "V", "3");
    //VNCommonLabel_push_word(label, "you", "", "", "");
}
