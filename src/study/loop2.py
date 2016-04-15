#!/bin/env python
str = raw_input('Enter a string: ')
i = 0
while i < len(str):
    print str[i]
    i += 1

str = raw_input('Enter another string: ')
for c in str:
    print c
