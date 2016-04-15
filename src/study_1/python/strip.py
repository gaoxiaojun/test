#!/usr/bin/env python

from string import whitespace

def strip(s):
    t = ''
    i = 0
    length = len(s)
    inword = False
    for c in s:
        if not inword:
            if c in whitespace:
                pass
            else:
                inword = True
                t += c
        else:
            t += c
    s = t
    inword = False
    t = ''
    for c in reversed(s):
	if not inword:
	    if c in whitespace:
	        pass
	    else:
	        inword = True
	        t += c
	else:
	    t += c
    return reversed(t)
