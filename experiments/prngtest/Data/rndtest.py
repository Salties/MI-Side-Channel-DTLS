#!/usr/bin/python

import sys;

nbit = 64;
nbyte = nbit / 8;

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
        except:
            continue;

    #Total sample size.
    total = len(rndlist);

    #Count for zero bit for each position.
    bit0count = [0]*nbit;
    for rndval in rndlist:
        rndvalb = ('{:0'+str(nbit)+'b}').format(rndval);
        for b in range(0,nbit):
            if rndvalb[b] == '0':
                bit0count[b] += 1;

    for i in range(0, len(bit0count)):
        print 'Bit {}\t: {} / {} ({:.3f}%)'.format(i, bit0count[i], total, float(bit0count[i])/float(total)*100);

    total0count = sum(bit0count);
    print 'Total 0 bit: {} / {} ({:.5f}%)'.format(total0count, total*nbit, float(total0count) / float(total * nbit) * 100)

    #Count byte appearance.
    bytecount = [0] * 256;
    bytesum = 0;
    for rndval in rndlist:
        for i in range(0,nbyte):
            byteval = rndval & 0xff;
            rndval >>= 8;
            bytecount[byteval] += 1;
            bytesum += byteval;

    for i in range(0, len(bytecount)):
        print 'Byte {:02X}\t: {} / {} ({:.3f}%)'.format(i, bytecount[i], total*nbyte, float(bytecount[i])/float(total*nbyte) * 100);
    print 'Byte avg: {:.3f}'.format(float(bytesum)/float(total*nbyte));

    return;

if __name__ == "__main__":
    main(len(sys.argv),sys.argv);
