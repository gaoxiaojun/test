#!/usr/bin/env python

import sys
print >> sys.stderr, 'Fatal error: invalid input!'
logfile = open('/home/baiyu/study/mylog.txt', 'a')
print >> logfile, 'Fatal error: invalid input!'
logfile.close()
