#!/usr/bin/env python

num = raw_input('Enter the money you have(less than a dollar):')
money = int(num)
if money / 25.0 > 0:
    print '%d quarters' % (money / 25)
    money %= 25
if money / 10.0 > 0:
    print '%d dimes' % (money / 10)
    money %= 10
if money / 5.0 > 0:
    print '%d nickels' % (money / 5)
    money %= 5
if money != 0:
    print '%d pennies' % money
