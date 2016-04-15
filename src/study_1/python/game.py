#!/usr/bin/env python

import string
import random

choices = {'r':'rock', 'p':'paper', 's':'scissors'}
choice = ['r', 'p', 's']
rock_win = {'p':'lose', 's':'win'}
paper_win = {'s':'lose', 'r':'win'}
scissors_win = {'r':'lose', 'p':'win'}


def showMenu():
    print '''Welcome to rochambeau game, here are all the choices:
(r)ock
(p)aper
(s)cissors
'''

while 1:
    showMenu()
    select = raw_input('Enter your choice: ').strip()[0].lower()
    if select not in choice:
        print 'Invalid option, try again'
    else:
        computer_select = random.choice(choice)
        if select == computer_select:
	    print 'We are the same, %s' % choices[select]
	elif select == 'r':
	    print 'You are %s and I\'m %s, you %s' % (choices[select],
		    choices[computer_select], rock_win[computer_select])
	elif select == 'p':
	    print 'You are %s and I\'m %s, you %s' % (choices[select],
		    choices[computer_select], paper_win[computer_select])
	elif select == 's':
	    print 'You are %s and I\'m %s, you %s' % (choices[select],
		    choices[computer_select], scissors_win[computer_select])
    print
