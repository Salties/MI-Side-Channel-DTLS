#!/usr/bin/python

'''
README:
Picks where to have lunch.
'''

import os;
import sys;
import hashlib;
import time;

#Defualts
candidates = "candidates.txt";
logfile = "lunchlog.log";

def GetLastline(fd):
    lines = fd.readlines();
    if lines:
        return lines[-1];
    else:
        return None;

def ReadCandidates(canfilename):
    #Read candidates list.
    try:
        canfile=open(canfilename, "r");
    except IOError:
        print("Error: Candidates file ({}) not found.".format(canfilename));
        return -1;
    
    canlist = canfile.readlines();
    for i in range(0,len(canlist)):
        canlist[i] = canlist[i].replace("\n",'');

    print "#Candidates: {}".format(canlist);

    return canlist;

def GetExclude():
    excludelist = list();
    #Exclude last pick.
    logfd = open(logfile, "w+r");
    lastline = GetLastline(logfd);
    if lastline != None:
        lastpick = GetLastline(logfd).split()[-1];
        print "#Last pick: {}".format(lastpick);
        excludelist.append(lastpick);
    print "#Excluding: {}".format(excludelist);
    logfd.close();
    return excludelist;

def PickToday(canlist, exclude, seed):
    #Remove excluded candidates.
    for i in exclude:
        while i in canlist:
            canlist.remove(i);
            print "#Excluded: {}.".format(i);
    print "#Available: {}".format(canlist);
    print "#{} choies.".format(len(canlist));
    #Hash by SHA256.
    randnum = int(hashlib.sha256(seed).hexdigest(),16);
    print "#SHA256: {:X}".format(randnum);
    pick = randnum % len(canlist);
    print "#Pick = {}".format(pick);
    return pick;

def LogLunch(lunch):
    logfd = open(logfile, "w+r");
    date = time.strftime("%Y/%m/%d");
    #Format: "DATE: PICK"

    logmessage = "{}: {}\n".format(date, lunch);
    if logmessage != GetLastline(logfd):
        logfd.write(logmessage);
    return;

def main(argc, argv):
    #Read candidates list.
    canlist = ReadCandidates(candidates);
    if canlist == -1:
        return -1;
    #Exclude what we don't want to go.
    exclude = GetExclude();
    #Pick what we are going to have today!
    date = time.strftime("%d/%m/%Y");
    pick = PickToday(canlist, exclude, date);
    lunch = canlist[pick];
    print("Today's Pick: {}".format(lunch));
    #Log the pick of today.
    LogLunch(lunch);

    return 0;

if __name__ == "__main__":
    main(len(sys.argv),sys.argv);
    exit();
