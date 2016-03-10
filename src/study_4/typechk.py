#!/usr/bin/env python

def displayNumType(num):
    print num, 'is',
    if isinstance(num, (int, long, float, complex)):
	print 'a number of type:', type(num).__name__
    else:
	print 'not a number at all!'

displayNumType(-69)
displayNumType(1L)
displayNumType(98.6)
displayNumType(0+0j)
displayNumType('xx')
