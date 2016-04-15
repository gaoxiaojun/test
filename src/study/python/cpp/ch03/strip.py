#!/bin/env python
import string
fname = raw_input("Enter file name: ")
fp = open(fname, 'r')
for s in fp:
    print string.strip(s)
