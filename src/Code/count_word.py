#!/usr/bin/env python
#-*- coding: utf-8 -*

word_dict = {}
with open("count_word.py") as f:
    for line in f:
        for c in line:
            if not c.isalpha():
                line = line.replace(c, ' ')
        print line

        for word in line.split():
            if word_dict.has_key(word):
                word_dict[word] += 1
            else:
                word_dict[word] = 1

for key in word_dict.keys():
    print key, word_dict[key]
