#!/usr/bin/python3

import traces2
import sys

def main(argc, argv):
    input = argv[1]
    fd = open(argv[2], 'wb')
    if argc > 3:
        start = int(argv[3])
        end = int(argv[4])
        ts = traces2.TraceSet(input, start, end, showprogress = True)
    else:
        ts = traces2.TraceSet(input, showprogress = True)
    ts.Dump(fd)
    fd.close()

    return

if __name__ == '__main__':
    main(len(sys.argv), sys.argv)

