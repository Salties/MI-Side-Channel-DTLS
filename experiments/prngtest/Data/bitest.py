#!/usr/bin/python

import sys;

nbit = 16;
bitwidth = 0;
window = 20000000;

ContikiMsg= "\
Contiki-3.0\
TI SmartRF06 + cc2538EM\
 Net: sicslowpan\
 MAC: CSMA\
 RDC: ContikiMAC\
Rime configured with address 00:12:4b:00:04:1e:af:c6\
"

#Main function
def main(argc, argv):
    #Open log file.
    datafile = open(argv[1]);
    rndbitstream = '';
 
    bitfile = -1;
    if argc > 2 :
        bitfile = open(argv[2],'w');

    #Read in data.
    rndnum = 1;
    rndlist = list();
    while rndnum:
        rndnum = datafile.readline();
        if rndnum == '\n':
            continue;
        if rndnum in ContikiMsg:
            continue;
        if rndnum == '':
            break;
        if rndnum[0] == '#':
            continue;
        #Remove illegal chracters.
        rndnum.replace('\0','');
        rndnum.replace(' ','');
        rndnum.replace('\n','');
        rndnumlen = len(rndnum);

        if rndnumlen > 4:
            rndnum = rndnum[:(rndnumlen - 3)];
        #print rndnum; #DEBUG info.

        #Interpret data as heximal.
        try:
            rndval = int(rndnum,16);
            rndlist.append(rndval);
            rndbitstream += ('{:0'+str(nbit)+'b}').format(rndval);
        except:
            continue;

    #Total sample size.
    total = len(rndbitstream);
    print '#Total: {}'.format(total);
    nzero = rndbitstream.count('0');
    print '#{}/{} {:03f}%'.format(nzero, total - nzero, float(nzero)/float(total));

    if bitfile != -1:
        if bitwidth == 0:
            bitfile.write(rndbitstream);

        else:
            i = 0;
            while i < len(rndbitstream):
                bitfile.write(rndbitstream[i:i+bitwidth]);
                i += bitwidth;

    return;

if __name__ == "__main__":
    main(len(sys.argv),sys.argv);
