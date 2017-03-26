#!/usr/bin/python

import sys;
import csv;

#Default settings.
logfile=0;
timeout = 10000;
client = 'aaaa::1';
server = 'aaaa::200:0:0:2';
lenspec=list();
keywords=list();
protocols=list();

class Record:
    def __init__(self, packet_dump):
        #Split csv lines by comma.
        segments = packet_dump.split(',');

        # A captured packet of cooja is of the form:
        self.no = int(segments[0].strip('"'));
        self.time = 1000 * float(segments[1].strip('"'));
        self.src = segments[2].strip('"');
        self.dst = segments[3].strip('"');
	self.protocol = segments[4].strip('"');
	self.length = segments[5].strip('"');
	self.info = segments[6].strip("\n").strip('"');
	if(len(segments) > 6):
	    self.other = segments[7:];
        return;

    def IsData(self, lenspec=[]):
	#Ignore all broadcast packets.
        if self.dst is '-': #FIXME: Replace '-' with broadcast address.
	    return False;

        #Filter packets with specific protocols.
        if len(protocols) is not 0:
            if self.protocol in protocols:
                       return True;
            return False;
 
        #A packet contains any of the keywords are valid.
        if len(keywords) is not 0:
            for key in keywords:
                if self.info.find(key) is not -1:
                    return True;
            return False;


	#Reserve packets with specific length if lenspec is specified. This can remove the handshake packets.
	if lenspec:
	    if not self.length in lenspec:
		return False;

	return True;
    
    def PrintRecord(self):
	#Print the packet. MAC Protocol is ignored.
        print('#%010f\t%s\t => \t%s\t%s\t%s\t%s' %\
	(self.time, self.src, self.dst, self.length, self.protocol, self.info));
	return;
    
    def __del__(self):
        return;

def PrintHelp():
    print "Usage: Extract time intervals from a wireshark csv.";
    print "getintervals.py CSVFILE [-c CLIENT[=%s]] [-s SERVER[=%s]]  [-p PROTOCOLS] [-k KEYWORD] [-t TIMEOUT[=500]] [-l LENGTHSPEC]" % (client, server);
    exit();

def Init():
    global logfile, client, server, protocols, keyword, timeout, lenspec;

    if len(sys.argv) <= 1 or '-h' in sys.argv or '--help' in sys.argv:
	PrintHelp();
    
    #Parse command line arguments.
    cmdargs = sys.argv;
    
    #Open a cooja radio log specified by command line argument.
    logfile = open(cmdargs[1], 'r');
    del(cmdargs[1]);
	
    #Set client. (Optional)
    if '-c' in cmdargs:
	index = cmdargs.index('-c');
	client = cmdargs[index + 1];
	del(cmdargs[index : index + 2]);

    #Set server. (Optional)
    if '-s' in cmdargs:
	index = cmdargs.index('-s');
	server = cmdargs[index + 1];
	del(cmdargs[index : index + 2]);
 
 #Specify keyword in packet info. (Optional)
    while '-p' in cmdargs:
        index = cmdargs.index('-p');
        protocols.append(cmdargs[index + 1]);
        del(cmdargs[index : index + 2]);
  
    #Specify keyword in packet info. (Optional)
    while '-k' in cmdargs:
        index = cmdargs.index('-k');
        keywords.append(cmdargs[index + 1]);
        del(cmdargs[index : index + 2]);
    
    #Set maximum timeout. (Optional)
    if '-t' in cmdargs:
	index = cmdargs.index('-t');
	timeout = int(cmdargs[index + 1]);
	del(cmdargs[index : index+2]);

    #Specify keyword in packet info. (Optional)
    while '-l' in cmdargs:
        index = cmdargs.index('-l');
        lenspec.append(cmdargs[index + 1]);
        del(cmdargs[index : index + 2]);
  
    return;

def GetResponseIntervals(records, client, server):
    ri = list();
    #If a packet from client to server is followed by a packet from server to client, 
    #then we consider them to be a request and a response.
    i = 0;
    lastreq = lastrpy = 0;
    #FIXME: Searching is a substring matching problem... and I'm not doing the fancy algorithms here.
    while i < len(records): 
        src = records[i].src;
        dst = records[i].dst;
	#If the packet is client => server, mark it as request.
	if (src == client) and (dst == server):
	    lastreq = i;
	#If the packet is server => client, mark it as response. 
	elif (src == server) and (dst == client):
	    lastrpy = i;
	#If a request is followed by a response, we mark them as a session.
	if lastrpy - lastreq is 1:
	    #Then compute the response interval as their time difference.
	    respintv = records[lastrpy].time - records[lastreq].time;
	    #Record it if within timeout.
	    if respintv <= timeout:
		ri.append(respintv);
	i += 1;
    
    return ri;
    
#Main
Init();

#Read packets from the log line by line.
records = list();
#Read out the first line of column names.
newline = logfile.readline();
while True:
    newline = logfile.readline();
    if not newline: 
        break; #EOF
    #Retrive the packet dump.
    newrecord = Record(newline);
    if newrecord.IsData(lenspec):
	records.append(newrecord);


#Print filtered packets.    
for rec in records:
    rec.PrintRecord();

#Extrac response interval from records.
ris = GetResponseIntervals(records, client, server);
#print "RIs =\n";
for ri in ris:
    print "%.3f" % ri
print "";

exit();
