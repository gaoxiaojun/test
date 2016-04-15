#!/usr/bin/env python

import random

def display(aList, col):
    length =  len(aList)
    remainder = length % col
    quotient = length / col

    i = 0
    j = 0
    k = 0
    while i < quotient:
        while j < col:
	    print '%3d' % aList[k],
	    j += 1
	    k += 1
	print
	j = 0
	i += 1
    blanks = col - remainder
    while blanks > 0:
        print ' ' * 3,
	blanks -= 1
    while k < length:
        print '%3d' % aList[k],
	k += 1
    print

def v_display(aList, col):
    length = len(aList)
    remainder = length % col
    quotient = length / col

    v_list = []
    i = j = 0
    k = quotient
    l = 0
    while l < col - remainder:
        v_list[k][l] = None
	l += 1
    while i < quotient + 1:
        while j < col:
	    
	    
    i = j = k = 0
    blanks = col - remainder
    while i < quotient:
        while j < col:
	    if j < blanks:
	        print '%3d' % aList[i + j * quotient],
	    else:
	        print '%3d' % aList[i + j * (quotient + 1)],
	    j += 1
	print
	j = 0
	i += 1
    blank = blanks
    while blank > 0:
        print ' ' * 3,
	blank -= 1
    the_last = col - remainder
    while the_last < col:
        #print '%3d' % aList[i + the_last * (quotient+1)],
	the_last += 1
    print

while 1:
    select = raw_input('have time?(y/n): ')
    if select != 'y':
        break
    all = int(raw_input('Enter the number of numbers(0-100): '))
    col = int(raw_input('Enter the display column: '))

    i = 0
    aList = []
    while i < all:
        aList.append(random.randint(0, 100))
        i += 1
    display(aList, col)
    choice = raw_input('Want to sort(y/n):')
    if choice == 'y':
        while 1:
            choice = raw_input('horizontal sorting or vertical sorting(h/v), \nany other keys to quit sorting: ')
	    aList.sort()
	    if choice == 'h':
	        display(aList, col)
	    elif choice == 'v':
	        v_display(aList, col)
	    else:
		break
