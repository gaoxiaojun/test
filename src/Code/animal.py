#!/usr/bin/env python
#-*- coding: utf-8 -*-

class Animal(object):
    def __init__(self, name):
        self.name = name
    def bark(self):
        print self.name, 'says: ah'

class Cat(Animal):
    def __init__(self, name):
        super(Cat, self).__init__(name)
    def bark(self):
        print self.name, 'says miaomiao'

class Dog(Animal):
    def __init__(self, name):
        super(Dog, self).__init__(name)
    def bark(self):
        print self.name, 'says wangwang'

def main():
    cat = Cat('joe')
    dog = Dog('pik')
    cat.bark()
    dog.bark()

if __name__ == '__main__':
    main()
