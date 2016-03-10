#!/usr/bin/env python

'''pw.py is a simple random password generator. It receives a number
argument as the length of password which you want to generate.'''

import os
import sys
from string import ascii_letters, punctuation, digits
from random import choice

def rand_passwd():
    try:
        s = ascii_letters + punctuation + digits
        return ''.join([choice(s) for i in range(int(sys.argv[1]))])
    except IndexError, e:
        print str(e)
    except ValueError:
        print __doc__
        print 'Usage: %s n-digit-password' % os.path.basename(sys.argv[0])

def main():
    passwd = rand_passwd()
    if passwd is not None:
        print passwd

if __name__ == '__main__':
     main() 
