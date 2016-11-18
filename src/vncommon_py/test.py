#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2016 truong-d <truong-d@truongd-ThinkPad-X1-Carbon-3rd>
#
# Distributed under terms of the MIT license.

"""

"""
# from vncommon import VNCommonLabel
import vncommon
# from vncommon import VNCommonLabel_push_word, VNCommonLabel_print

# a = "20 2/3/2009"
# a = "20 22/3"
# print vncommon.normalized_date(a)
a = "20h20p"
print vncommon.normalized_time(a)
a = "18h"
print vncommon.normalized_time(a)
a = "20p"
print vncommon.normalized_time(a)
a = "8:20"
print vncommon.normalized_time(a)

a = "2m"
print vncommon.normalized_unit(a)

a = "2kg"
print vncommon.normalized_unit(a)

a = "2,1kg"
print vncommon.normalized_unit(a)

a = "2,1g"
print vncommon.normalized_unit(a)

a = "2,1 fdjsag"
print vncommon.normalized_unit(a)

a = "17h00"
print vncommon.normalized_number(vncommon.normalized_time(a))
# label = vncommon.VNCommonLabel()
# VNCommonLabel_push_word(label, "hello", "h e", 'A', 'B-A', '1')
# VNCommonLabel_push_word(label, "hello", "h e", 'A', 'B-A', '1')
# a = VNCommonLabel_print(label)
# print(a)
