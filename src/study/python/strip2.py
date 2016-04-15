#!/usr/bin/env python

from string import whitespace

def strip(s):
    l_index = 0
    r_index = -1
    length = len(s)
    while l_index < length:
        if s[l_index] not in whitespace:
            break
        l_index += 1
    while r_index >= -length:
        if s[r_index] not in whitespace:
            break
        r_index -= 1
    if r_index == -1:
        return s[l_index:]
    else:
        return s[l_index:r_index+1]
