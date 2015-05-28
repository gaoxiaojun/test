#!/usr/bin/env python

import os

flist = os.listdir(".")
for fname in flist:
    try:
        new_fname = ''.join(fname.split('Cache'))
    except ValueError:
        pass
    os.system("mv %s %s" % (fname, new_fname));
