#!/bin/env python
aList = [x for x in range(20)]
print aList
i = 0
sum = 0
while i < len(aList):
    sum += i
    i += 1
print 'sum of aList is %d, average is %f' % (sum, float(sum)/len(aList))

bList = [x for x in range(10)]
print bList
sum = 0
for i in bList:
    sum += i
print 'sum of bList is %d, average is %f' % (sum, float(sum)/len(bList))
