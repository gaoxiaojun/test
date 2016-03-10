#!/usr/bin/env python

from sys import argv
import os

try:
    origin_name = argv[1]
    copy_name = argv[1] + '.copy'
    os.system('cp %s %s' % (origin_name, copy_name))

    with open(copy_name) as f:
        lines = f.readlines()
        save_file = open(origin_name, 'w')

        for line in lines:
            spaces = ''
            for s in line:
                if s == ' ':
                    spaces += s
                else:
                    break
            save_file.write(spaces + ' '.join(line.split()) + '\n')

        save_file.close()
except IndexError, e:
    print 'Usage: %s file' % os.path.basename(argv[0])
