#!/usr/bin/env python

import random

total_num = random.randint(2, 100)
random_list = []
for i in range(total_num):
    random_list.append(random.randint(0, 2**31-1))
print random_list
sel_num = random.randint(1, 100)
if sel_num > total_num:
    print 'sample more than population'
else:
    sel_list = random.sample(random_list, sel_num)
    #print sel_list
    print sorted(sel_list)
