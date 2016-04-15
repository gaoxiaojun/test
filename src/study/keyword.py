#!/bin/env python
aTuple = ('and', 'as', 'assert', 'break', 'class', 'continue', 'def', 'del','elif', 'else', 'except', 'exec', 'finally', 'for', 'from', 'global', 'if', 'import', 'in', 'is', 'lamba', 'not', 'or', 'pass', 'print', 'raise', 'return', 'try', 'while', 'with', 'yield', 'None')
longest = 0
for s in aTuple:
    if len(s) > longest:
        longest = len(s)
    #print s,

width = 5
cnt = 0
for s in aTuple:
    if (cnt+1) % width == 0:
        print
	cnt = 0
    cnt += 1
    print (s + ' ' * (longest - len(s) + 1)),
