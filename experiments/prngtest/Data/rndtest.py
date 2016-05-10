#!/usr/bin/python

import sys;

nbit = 16

def main():
    datafile = open(sys.argv[1]);
    rndnum = 1;
    rndlist = list();
    rndcount = [0]*(0xffff);
    while rndnum:
        rndnum = datafile.readline().replace('\n','');
        if rndnum == '':
            break;
        rndval = int(rndnum,16);
        rndlist.append(rndval);
        rndcount[rndval] += 1;

    bit0count = [0]*nbit;

    for rndval in rndlist:
        rndvalb = '{:016b}'.format(rndval);
        for b in range(0,nbit):
            if rndvalb[b] == '0':
                bit0count[b] += 1;
        print rndvalb;

    total = len(rndlist);

    for i in range(0, len(bit0count)):
        print 'Bit {}\t: {} / {} ({:.3f}%)'.format(i, bit0count[i], total, float(bit0count[i])/float(total)*100);

    return;

if __name__ == "__main__":
    main();
