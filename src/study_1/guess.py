#!/bin/env python
criteria = 58
guess = raw_input('Enter your guess number: ')
while int(guess) != criteria:
     print 'Not correct, guess again'
     guess = raw_input('Enter your guess number: ')
if int(guess) == criteria:
    print 'Bingo!'
else:
    print 'Brute Force!!'
