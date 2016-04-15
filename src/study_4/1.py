#!/usr/bin/env python

aTuple = (1, 2, 3, 4, 5)
i = 0
aSum = 0
while i < len(aTuple):
    aSum += aTuple[i]
    i += 1
    
bSum = 0
for i in aTuple:
    bSum += i

print aSum, bSum

aList = []
aInput = raw_input('Enter a number: ')
while int(aInput) > 0:
    aList.insert(len(aList), int(aInput))
    aInput = raw_input('Enter a number: ')

i = 0
cSum = 0
while i < len(aList):
    cSum += aList[i]
    i += 1

dSum = 0
for i in aList:
    dSum += i

print cSum, dSum
