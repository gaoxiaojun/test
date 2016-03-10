#!/usr/bin/env python

sum_primes = 0
flags_list = [1] * 2000001
flags_list[0], flags_list[1] = 0, 0

for i in xrange(2, 2000001):
    if flags_list[i] == 0:
        continue
    sum_primes += i
    j = 2 * i
    while j < 2000001:
        flags_list[j] = 0
        j += i
print 'The sum of prime numbers below 2 million is', sum_primes
