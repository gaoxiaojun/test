#!/usr/bin/env python

def ShowMenu():
    print '''(S)um of five numbers
(A)verage of five numbers
(Q)uit.'''

while 1:
    ShowMenu()
    cin = raw_input('Enter your choice: ').lower()[0];

    if cin == 's':
	None
    elif cin == 'a':
	None
    elif cin == 'q':
	break
