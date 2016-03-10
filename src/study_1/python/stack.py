#!/usr/bin/env python

class stack(object):
    """ my first class implement """
    def __init__(self):	
	""" constructor """
	self.stack = []
    def push(self, obj):
	self.stack.append(obj)
    def pop(self):
	return self.stack.pop(-1)
