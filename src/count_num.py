#!/usr/bin/env python
from string import digits

f = open('/tmp/mylog.txt', 'r')
cnt = 0
for line in f:
    for c in line:
        if c in digits:
            cnt += 1
print 'Total %d numbers' % cnt
