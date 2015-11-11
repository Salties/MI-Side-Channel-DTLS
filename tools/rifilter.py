#!/usr/bin/python

import sys
import os.path

#Main
if len(sys.argv) < 2: 
    exit();
elif os.path.isfile(sys.argv[1]):
    fp = open(sys.argv[1], 'r');
else:
    fp = sys.stdin;

fltr = sys.argv[1:];
ris = fp.read().split();

#Remove RIs in filter.
i = 0;
while i < len(ris):
    if ris[i] in fltr:
	del ris[i];
	continue;
    i = i + 1;
    
for ri in ris:
    print "%s" % ri,;
print "";
