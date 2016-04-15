#!/usr/bin/env python
cnt = 0
sum = 0
while cnt <= 10:
    sum += cnt
    cnt += 1
print 'The sum of 0 to 10 inclusive is', sum

sum = 0
for cnt in range(11):
    sum += cnt
print 'for loop\'s result is', sum
