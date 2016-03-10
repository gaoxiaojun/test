#!/usr/bin/env python

def backword(s):
    length = len(s)
    i = -1
    t = s
    while i >= -length:
        t += s[i]
        i -= 1
    return t
