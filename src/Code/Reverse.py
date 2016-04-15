#!/usr/bin/env python
#-*- coding: utf-8 -*-

class Reverse(object):
    '''Iterator for looping over a sequence backwards.'''
    def __init__(self, data):
        self.data = data
        self.index = len(data)
    def __iter__(self):
        return self
    def __next__(self):
        if self.index == 0:
            raise StopIteration
        self.index -= 1
        return self.data[self.index]
