#!/usr/bin/python

import sys

#Main
fp = open(sys.argv[1], 'r');
fltr = sys.argv[2:];
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
