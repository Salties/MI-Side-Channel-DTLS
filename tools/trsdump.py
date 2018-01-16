#!/usr/bin/python3 -u

import traces
import sys
import gzip

iFile = None
oFile = None
compressed = False
start = 0
end = float('inf')


def PrintHelp():
    print('Dump trs files as TraceSet object.'),
    print('Usage: trsdump.py [-c] [-s START] [-e END] TRSFILE DUMPFILE')
    print('\t-t : Print this message.')
    print('\t-c : Use gzip compression.')
    print('\t-s : Use points from START in each trace.')
    print('\t-e : Use points from END in each trace.')
    return


# Parse command line arguments.
def ParseArgs(argc, argv):
    global iFile, oFile, compressed, start, end

    if argc < 3:
        return False

    i = 1
    while i < argc:
        if argv[i] == '-h':  # Print help message.
            PrintHelp()
            exit(0)

        elif argv[i] == '-c':  # Use gzip compress.
            compressed = True

        elif argv[i] == '-s':   # Set trace start.
            start = int(argv[i + 1])
            i += 1

        elif argv[i] == '-e':    # Set trace end.
            end = int(argv[i + 1])
            i += 1

        elif iFile == None:  # First non optional argument as input file.
            iFile = argv[i]

        elif oFile == None:  # Second non optional argument as output file.
            oFile = argv[i]

        else:
            print('Unknown option:', argv[i])
            pass

        i += 1

    return True


def main(argc, argv):
    if not ParseArgs(argc, argv):
        PrintHelp()
        return -1

    if compressed:
        fd = gzip.open(oFile, 'wb')
    else:
        print('#')
        fd = open(oFile, 'wb')

    try:
        ts = traces.TraceSet(
            iFile, start, end, showprogress=True, showheader=True)
        ts.Dump(fd)
    finally:
        fd.close()

    print('Dump trs as TraceSet object (ver: {})'.format(ts.version))

    return


if __name__ == '__main__':
    main(len(sys.argv), sys.argv)
