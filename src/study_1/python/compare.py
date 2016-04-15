#!/usr/bin/env python

def cmp(s1,s2):
    len1 = len(s1)
    len2 = len(s2)
    if len1 > len2:
        length = len1
    else:
        length = len2

    i = 0
    while i < length:
        if s1[i] != s2[i]:
	    return ord(s1[i]) - ord(s2[i])
	elif i == len1-1 or i == len2-1:
	    break
	i += 1
    if len1 < len2:
        return -1
    elif len1 == len2:
        return 0
    else:
        return 1
