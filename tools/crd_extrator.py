#!/usr/bin/python

'''
README:
This program extract traffic features from cooja simulator radio dump (with '6lowPAN analyser with PCAP' option).
'''

import sys;

#Default settings.
logfile=0;
timeout = 500;
client = 1;
server = 2;
lenspec=list();

class Record:
    def __init__(self, packet_dump):
        segments = packet_dump.split();
        # A captured packet of cooja is of the form:
        # 'Time'+'Src_ID'+'Dts_ID'+'Length'+'Mac_Protocol'+'Packet_Type'+'Src_Addr'+'Dst_Addr'...
        self.time = int(segments[0]);
        self.srcid = segments[1];
        self.dstid = segments[2];
        self.length = segments[3].replace(':',''); # Remove the last colon in length filed.
	self.macproto = segments[4];
	self.packtype = segments[5];
	self.others = segments[6:];
        return;
	
    def IsData(self, lenspec=[]):
	#Ignore all MAC ACK packets.
	if self.packtype is 'A':
	    return False;
	#Ignore all RPL packets.
	if 'RPL' in self.others:
	    return False;
	#Ignore all broadcast packets.
	if self.dstid is '-':
	    return False;
	#Reserve packets with specific length if lenspec is specified. This can remove the handshake packets.
	if lenspec:
	    if not self.length in lenspec:
		return False;

	return True;
    
    def PrintRecord(self):
	#Print the packet. MAC Protocol is ignored.
	print('%s %s %s %s %s %s' %\
	(self.time, self.srcid, self.dstid, self.length, self.packtype, self.others));
	return;
    
    def __del__(self):
        return;

def PrintHelp():
    print "Usage: Extract packet features from a cooja radio dump.";
    print "crd_extractri.py LOGFILE [-c CLIENT_ID[=1]] [-s SERVER_ID[=2]] [-t TIMEOUT[=500]] [LENGTHSPEC]";
    exit();

def Init():
    global logfile, timeout, client, server, lenspec;
    if len(sys.argv) <= 1 or '-h' in sys.argv or '--help' in sys.argv:
	PrintHelp();
    
    #Parse command line arguments.
    cmdargs = sys.argv;
    
    #Open a cooja radio log specified by command line argument.
    logfile = open(cmdargs[1], 'r');
    del(cmdargs[1]);

    #Set maximum timeout. (Optional)
    if '-t' in cmdargs:
	index = cmdargs.index('-t');
	timeout = int(cmdargs[index + 1]);
	del(cmdargs[index : index+2]);
	
    #Set client's node ID. (Optional)
    if '-c' in cmdargs:
	index = cmdargs.index('-c');
	client = cmdargs[index + 1];
	del(cmdargs[index : index + 2]);

    #Set server's node ID. (Optional)
    if '-s' in cmdargs:
	index = cmdargs.index('-s');
	server = cmdargs[index + 1];
	del(cmdargs[index : index + 2]);
    
    #Specify packet length. (Optional)
    lenspec = list();
    if len(cmdargs) > 1:
	lenspec = cmdargs[1:];
	
    return;


def GetResponseIntervals(records, client, server):
    ri = list();
    #If a packet from client to server is followed by a packet from server to client, 
    #then we consider them to be a request and a response.
    i = 0;
    lastreq = lastrpy = 0;
    #FIXME: Searching is a substring matching problem... and I'm not doing the fancy algorithms here.
    while i < len(records): 
	#If the packet is client => server, mark it as request.
	if (records[i].srcid == str(client)) and (records[i].dstid == str(server)):
	    lastreq = i;
	#If the packet is server => client, mark it as response. 
	elif (records[i].srcid == str(server)) and (records[i].dstid == str(client)):
	    lastrpy = i;
	#If a request is followed by a response, we mark them as a session.
	if lastrpy - lastreq is 1:
	    #Then compute the response interval as their time difference.
	    respintv = records[lastrpy].time - records[lastreq].time;
	    if respintv <= timeout:
		#Record it if within timeout.
		ri.append(respintv);
	i += 1;
    
    return ri;
    
#Main
Init();

#Read packets from the log line by line.
records = list();
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
ri = GetResponseIntervals(records, client, server);
print "Response Intervals = %s" % str(ri)

exit();
