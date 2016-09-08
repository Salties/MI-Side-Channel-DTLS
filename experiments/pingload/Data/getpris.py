#!/usr/bin/python

#Tool to calculate PRIs. Works for Wireshark >= 2.0.2.
#Grep the keywords "ICMPv6" and "reply" from the csv file before applying this tool.


import sys;

def main(argc, argv):
    filename = argv[1];

    packets = [];
    replies = [];
    requests = [];
    PRIs = [];

    fd = open(filename);

    #Read captured packet csv file.
    while True:
        packetinfo = fd.readline();
        if packetinfo == '':
            break;
        packets.append(packetinfo);

    #Compute PRIs reversely.
    i = len(packets) - 1;
    while i > 0:
        repliedtime = float(packets[i].split(',')[1].replace('\"',''));
        requestedtime = float(packets[i-1].split(',')[1].replace('\"',''));
        pri = 1000 * (repliedtime - requestedtime);
        PRIs.append(pri);
        i -= 2;

    #Reverse PRI back in normal order.
    i = len(PRIs) - 1;
    while i >= 0:
        print("{:.3f}".format(PRIs[i]));
        i -= 1;


    exit(0);

if __name__ == "__main__":
    main(len(sys.argv), sys.argv);
