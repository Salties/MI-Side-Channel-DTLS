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

    sessions = [];
    pris = [];

    fd = open(filename);
    fd.readline(); #Read out title line.

    #Read captured packet csv file.
    while True:
        packetinfo = fd.readline();
        if packetinfo == '':
            break;
        for i in keywords:
            if i not in packetinfo:
                continue;

        #Scan requests.
        replyseq = search('(reply in {})', packetinfo);
        if replyseq: #This is a request.
            i = PingSession(int(replyseq[0]), float(packetinfo.split(',')[1].replace('\"','')));
            sessions.append(i);
        #Record reply time.
        else: #This is a reply.
            packetattr = packetinfo.split(',');
            seqno = int(packetattr[0].strip('\"'));
            replytime = float(packetattr[1].strip('\"'));
            for i in sessions:
                if i.seqno == seqno:
                    i.responsetime = replytime;
                    pri = 1000*(i.responsetime - i.requesttime);
                    #print "ReplySeq={}, RequestTime={}, ResponseTime={}, PRI={}".format( i.seqno, i.requesttime, i.responsetime, pri);
                    print pri;
                    sessions.remove(i);
                    break;
    exit(0);

if __name__ == "__main__":
    main(len(sys.argv), sys.argv);
