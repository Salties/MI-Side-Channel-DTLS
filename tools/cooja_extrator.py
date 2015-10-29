#!/usr/bin/python

import sys

class Record:
    def __init__(self, packet_dump):
        segments = packet_dump.split();
        # A captured packet of cooja is of the form:
        # 'Time'+'Src_ID'+'Dts_ID'+'Length'+'Mac_Protocol'+'Packet_Type'+'Src_Addr'+'Dst_Addr'...
        self.time = segments[0];
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
    if len(sys.argv) <= 1 or '-h' in sys.argv or '--help' in sys.argv:
	print "Usage: Extract packets from a cooja radio dump.";
	print "extractri.py LOGFILE [-c CLIENT_ID[=1]] [-s SERVER_ID[=2]] [-t TIMEOUT[=500]] [LENGTH SPEC]";
	exit();
    return;
    
#Main
PrintHelp();

#Parse command line arguments.
cmdargs = sys.argv;

#Open a cooja radio log specified by command line argument.
logfilename = cmdargs[1];
logfile = open(logfilename, 'r');
del(cmdargs[1]);

#Set client's node ID. (Optional)
if '-c' in cmdargs:
    index = cmdargs.index('-c');
    client = cmdargs[index + 1];
    del(cmdargs[index : index + 2]);
else:
    client = 1;

#Set server's node ID. (Optional)
if '-s' in cmdargs:
    index = cmdargs.index('-s');
    server = cmdargs[index + 1];
    del(cmdargs[index : index + 2]);
else:
    server = 1;

#Set maximum timeout. (Optional)
if '-t' in cmdargs:
    index = cmdargs.index('-t');
    timeout = cmdargs[index + 1];
    del(cmdargs[index : index+2]);
else:
    timeout = 500;

#Specify packet length. (Optional)
lenspec = list();
if len(cmdargs) > 1:
    lenspec = cmdargs[1:];
    
#Start the main process.
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

exit();
