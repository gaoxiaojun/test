#!/usr/bin/env python
#-*- coding: utf-8 -*-

words = []
with open('uniq_word.py') as f:
    for line in f:
        words.extend(line.split())

uniq_words = set(words)
with open('uniq_word.txt', 'w') as f:
    f.write(str(uniq_words))
    f.write('\n')
