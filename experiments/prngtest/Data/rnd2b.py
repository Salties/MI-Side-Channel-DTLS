#!/usr/bin/python

import sys;

nbit = 16;
bitwidth = 16;

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

    #Read in data.
    rndnum = 1;
    rndlist = list();
    while rndnum:
        rndnum = datafile.readline().replace('\n','');
        if rndnum in ContikiMsg:
            continue;
        if rndnum == '':
            break;
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

    i = 0;
    while i < len(rndbitstream):
        print rndbitstream[i:i+bitwidth];
        i += bitwidth;



    return;

if __name__ == "__main__":
    main(len(sys.argv),sys.argv);
