#!/usr/bin/python3

import os;
import sys;
import subprocess;

#from joblib import Parallel, delayed
#import multiprocessing
import multiprocessing
from multiprocessing import Pool

#Lookup table.
class KeySigPair:
    key = 0
    sig = 0

    def __init__(self, inputkey, inputsig):
        self.key = inputkey;
        self.sig = inputsig;

def ComputeEntry(key):
#    key = ''.join(prngstream[idx:(idx+32)]);
    process = subprocess.Popen(["secp256r1mult", key], stdout = subprocess.PIPE);
    out, err = process.communicate();
    sig = out.decode("utf-8").replace('\n','');
    return KeySigPair(key,sig);
   
#Configurations
order = 1024; #Order of cyclic PRNG group.
parallel = 1;

#Main()
prngfile = open(sys.argv[1]);

prngstream = list();
prng = 1;
while prng:
    prng = prngfile.readline().replace('\n','');
    prngstream.append(prng);

keylist = list();
for i in range(0,order):
    keylist.append(''.join(prngstream[i:(i+32)]));

lookuptbl = list();
if not parallel:
    #Non parallel...
    print("#Using non-parallel method...");
    for i in range(0,order):
       lookuptbl.append(ComputeEntry(keylist[i]));
else:
    #PARALLEL!!!!!!!
    print("#Using parallel method...");
    num_cores = multiprocessing.cpu_count();
    print("#Number of cores:%d" % num_cores);
    pool = Pool(num_cores);
    lookuptbl = pool.map(ComputeEntry,keylist);

for idx,entry in enumerate(lookuptbl):
    print("#Key %d:\n %s %s" % (idx, entry.key, entry.sig));
