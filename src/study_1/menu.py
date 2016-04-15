#!/bin/env python
quit = 'x'
def displayMenu():
    print '*' * 40
    print '(1) sum of five numbers'
    print '(2) average of five numbers'
    print '(x) quit'
    print '*' * 40
def sumList(aList):
    sum = 0
    for i in aList:
        sum += i
    return sum
def average(aList):
    sum = sumList(aList)
    return float(sum)/len(aList)

displayMenu()
aList = [x for x in range(20, 70, 10)]
input = raw_input('Enter your choice: ')
while input != quit:
    if input == '1':
        print 'the sum is', sumList(aList)
    elif input == '2':
        print 'the average is', average(aList)
    displayMenu()
    input = raw_input('Enter your choice: ')
print 'Done!'
