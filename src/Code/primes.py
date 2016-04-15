#!/usr/bin/env python
#-*- coding:utf-8 -*-

class Prime(object):
    def __init__(self, n):
        self.primes = [2]
        self.top = n+1

    def find_prime(self):
        for i in xrange(3, self.top, 2):
            for j in self.primes:
                if i % j == 0:
                    break;
            else:
                self.primes.append(i)

    def show_prime(self):
        print self.primes
        print len(self.primes), 'prime numbers.'

def main():
    p = Prime(100)
    p.find_prime()
    p.show_prime()

if __name__ == '__main__':
    main()
