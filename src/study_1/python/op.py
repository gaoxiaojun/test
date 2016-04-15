#!/usr/bin/env python

while 1:
    aList = raw_input('Enter an equation(num1 op num2, q to exit):').split()
    if aList[0] == 'q':
        break
    num1, op, num2 = int(aList[0]), aList[1], int(aList[2])
    if op == '+':
        print num1 + num2
    elif op == '-':
        print num1 - num2
    elif op == '*':
        print num1 * num2
    elif op == '**':
        print num1 ** num2
    elif op == '/':
        print num1 / num2
    elif op == '%':
        print num1 % num2
