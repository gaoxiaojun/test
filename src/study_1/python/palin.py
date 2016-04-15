#!/usr/bin/env python

def palin(s):
    i = 0
    length = len(s)
    while i <= length / 2:
        if s[i] != s[length-i-1]:
            return False
        i += 1
    return True
