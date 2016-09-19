#!/usr/bin/python

#Tool to calculate PRIs. Works for Wireshark >= 2.0.2.
#Grep the keywords "ICMPv6" and "reply" from the csv file before applying this tool.


import sys;
from parse import *;

keywords = ["Echo(ping)", "reply"];

class PingSession:
    seqno = 0;
    requesttime = 0;
    responsetime = 0;

    def __init__(self, seq, reqtime):
        self.seqno = seq;
        self.requesttime = reqtime;

def main(argc, argv):
    filename = argv[1];

    packets = [];

    fd = open(filename);
    fd.readline(); #Read out title line.

    #Read captured packet csv file.
    while True:
        packetinfo = fd.readline();
        if packetinfo == '':
            break;
        packets.append(packetinfo);

    #Reverse search for Reply-ACK
    i = len(packets) - 1;
    while i >= 0:
        start = 0;
        end = 0;
        #Found a reply.
        if "Echo (ping) reply" in packets[i]:
            end = float(packets[i].split(',')[1].strip('\"'));
            #Keep looking for corresponding ACK.
            i -= 1;
            while i >= 0:
                #Found an ACK.
                if "Ack" in packets[i]:
                    start = float(packets[i].split(',')[1].strip('\"'));
                    print '{:.3f}'.format(1000*(end - start));
                    break;
                i -= 1;
        i -= 1;


    exit(0);
    return;

if __name__ == "__main__":
    main(len(sys.argv), sys.argv);
