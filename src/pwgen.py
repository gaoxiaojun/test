#!/usr/bin/env python
import random
from string import ascii_letters, digits
from sys import argv

alnum = ascii_letters + digits
pw_len = int(argv[1]);

pw = ""
random.seed()
while pw_len > 0:
    pw += random.choice(alnum)
    pw_len -= 1

print pw
