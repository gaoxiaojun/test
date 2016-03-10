#!/usr/bin/env python

import random

m = 3
n = 4

i = j = 0
matriceM_N = []
while i < m:
    while j < n:
        matriceM_N[i][j] = random.randint(0, 100)
        print '%3d' % matriceM_N[i][j],
        j += 1
    print
    i += 1
    j = 0

i = j = 0
matriceN_M = []
while i < n:
    while j < m:
        matriceN_M[i][j] = random.randint(0, 100)
        print '%3d' % matriceN_M[i][j]
        j += 1
    i += 1
    j = 0
    print

i = j = k = 0
product = []
while i < m:
    while k < m:
        while j < n:
            sum += matriceM_N[i][j] * matriceN_M[j][k]
	    j += 1
	product[i][k]

