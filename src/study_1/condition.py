#!/bin/env python
def check(number):
    if number > 0:
        print number, 'is a positive number'
    elif number == 0:
        print 'zero'
    else:
        print number, 'is a negative number'
number = raw_input('Enter a number: ')
check(int(number))
