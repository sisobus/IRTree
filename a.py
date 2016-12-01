#!/usr/bin/python
#-*- coding:utf-8 -*-
import random

keywords = ['wine','beer','soju']
def getRandomNumber():
    return random.randint(0,100000)

def getKeyword():
    return keywords[random.randint(0,2)]

if __name__ == '__main__':
    with open('random.txt','w') as fp:
        for i in xrange(100000):
            s = '%d %d %s %s %s'%(getRandomNumber(),getRandomNumber(),getKeyword(),getKeyword(),getKeyword())
            fp.write(s+'\n')
