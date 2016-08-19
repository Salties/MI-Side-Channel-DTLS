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
date = time.strftime("%Y/%m/%d"); #YY/MM/DD

def GetLastPick(fd):
    lastpick = "None";
    lines = fd.readlines();
    i = len(lines) - 1;
    while i >= 0:
        if lines[i].split()[0] == (date + ':'):
            i -= 1;
        else:
            lastpick = lines[i].split()[-1];
            break;
    return lastpick;

def ToMenuList(menulist):
    menustr = ', '.join(menulist);
    if menustr == '':
        return "NONE";
    else:
        return menustr;

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

    #Sort candidates list and print.
    canlist.sort();
    print "#Candidates: {}".format(ToMenuList(canlist));

    return canlist;

def GetExclude():
    excludelist = list();
    lastpick = "None";
    #Exclude last pick.
    try:
        logfd = open(logfile, "r");
        lastpick = GetLastPick(logfd);
        logfd.close();
    except IOError:
        pass;

    print "#Last pick: {}".format(lastpick);
    excludelist.append(lastpick);
    print "#Excluding: {}".format(ToMenuList(excludelist));
    return excludelist;

def PickToday(canlist, exclude, seed):
    #Remove excluded candidates.
    for i in exclude:
        while i in canlist:
            canlist.remove(i);
    print "#Available[{}]: {}".format(len(canlist),ToMenuList(canlist));
    #Hash by SHA256.
    randnum = int(hashlib.sha256(seed).hexdigest(),16);
    print "#SHA256: {:X}".format(randnum);
    pick = randnum % len(canlist);
    print "#Pick = {}".format(pick);
    return pick;

def LogLunch(lunch):
    logfd = open(logfile, "ar+");
    lines = logfd.readlines();
    #Format: "DATE: PICK"
    logmessage = "{}: {}\n".format(date, lunch);
    #Write when the log file is empty.
    if len(lines) == 0:
        logfd.write(logmessage);
    #Ignore duplicated log.
    elif logmessage != lines[-1]:
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
    pick = PickToday(canlist, exclude, date);
    lunch = canlist[pick];
    print("Today's Pick: {}".format(lunch));
    #Log the pick of today.
    LogLunch(lunch);
    return 0;

if __name__ == "__main__":
    main(len(sys.argv),sys.argv);
    exit();
