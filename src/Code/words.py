#!/usr/bin/env python
#-*- coding: utf-8 -*-

def count_words(filename):
    words = []
    with open(filename) as f:
        for line in f:
            words.extend(line.split())
    #print words
    word_count = {}
    for word in words:
        word_count[word] += 1

    print word_count

count_words('words.py')
