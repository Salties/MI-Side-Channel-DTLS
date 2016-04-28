#!/usr/bin/python

import sys

def GetLeadingZero(bistr):
    count = 0;
    while count < len(bistr):
        if bistr[count] != '0':
            break;
        count += 1;
    return count;


#Read ECDSA timing data from file.
datafile = open(sys.argv[1],'r');
datalist = list();

#Process line by line
while True:
    #Read a new line.
    line = datafile.readline();
    if not line:
        break;
    #Skip comment line.
    if line.strip()[0] == "#":
        continue;

    data = line.replace(" ",'').replace("\n",'').split(',');
    datalist.append(data);

#datalist = sorted(datalist, key=lambda x:x[1])

for data in datalist:
    datahex = data[0]; #Nonce
    databi = '{:0256b}'.format(int(datahex,16)); #Nonce as binari string.
    time = 0;
    if len(data) > 1:
        time = int(data[1]); #Execution time of nonce.
    hw = databi.count("1"); #Hamming weight of the nonce. 
    nzero = GetLeadingZero(databi); #Number of leading zeros of the nonce.
    print "%d\t%d\n" % (hw, time),;
