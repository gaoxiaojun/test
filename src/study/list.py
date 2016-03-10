#!/bin/env python
aList = [x for x in range(10)]
#aTuple = (y for y in range(10) if not x % 2)
aTuple = (1, 2, 3, 4, 5)
print aList
print aTuple
i = 0
sum = 0
while i < len(aList):
    sum += aList[i]
    i += 1
print 'aList\'s sum is', sum

sum = 0
for i in aTuple:
    sum += i
print 'aTuple\'s sum is', sum
