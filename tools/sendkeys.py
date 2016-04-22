#!/usr/bin/python -u
#Use no buffer python.

import sys
import time

if len(sys.argv) <= 1:
    print "Usage: sendkey.py KEYFILE"
    exit();

#Read test keys from command line argument.
keyfile = open(sys.argv[1], 'r');

key = 1;
while key:
    #Read a key from keyfile and remove "\n".
    key = keyfile.readline().replace("\n",'');
    print key;
    #Wait for curve computation.
    time.sleep(15);
