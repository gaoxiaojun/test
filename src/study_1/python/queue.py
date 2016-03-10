#!/usr/bin/env python

queue = []

def inqueue():
    queue.append(raw_input('Enter new string: ').strip())

def outqueue():
    if len(queue) == 0:
        print 'Cannot remove an element from an empty queue!'
    else:
        print 'Removed [' + queue.pop(0) + ']'

def viewqueue():
    print queue

CMDs = {'i': inqueue, 'o': outqueue, 'v': viewqueue}

def showmenu():
    pr = '(i)n\n(o)ut\n(v)iew\n\nEnter choice:'

    while True:
        while True:
            try:
	        choice = raw_input(pr).strip()[0].lower()
            except (EOFError, KeyboardInterrupt, IndexError):
		choice = 'q'
	    
	    print '\nYou picked: [%s]' % choice
	    if choice not in 'iovq':
	        print 'Invalid option, try again'
	    else:
	        break
	if choice == 'q':
	    break
	CMDs[choice]()

if __name__ == '__main__':
    showmenu()
