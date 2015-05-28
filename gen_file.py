#!/usr/bin/env python

import os

fname = "MapCache";
for i in range(65):
    name = fname + str(i) + ".Dat"
    os.system("touch %s" % name)
