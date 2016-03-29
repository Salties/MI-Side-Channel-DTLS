#!/usr/bin/python

import sys
import os.path

#Main
fp = sys.stdin; #Use stdin as default.
fltr = list();
if len(sys.argv) > 1:
    if os.path.isfile(sys.argv[1]):
        fp = open(sys.argv[1], 'r');
    else:
        fltr = sys.argv[1:];

ris = fp.read().split();

minrange = 12;
maxrange = 500;


#Remove RIs in filter.
ntotal = len(ris);
nvalid = 0;
i = 0;
while i < len(ris):
    if float(ris[i]) < minrange:
        del ris[i];
        continue;
    else:
        nvalid = nvalid + 1;
    i = i + 1;
 
for ri in ris:
    print "%.3f" % float(ri),;

sys.stderr.write("# %d / %d\n" % (nvalid, ntotal));
