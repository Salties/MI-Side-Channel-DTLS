import glob
import numpy
import scipy
import scipy.stats
import scipy.io
import random
import pickle
import sys
import struct
from tqdm import tqdm
import gzip

# Class of a single trace.


class Trace:
    def __init__(self, udata=None, points=None):
        self.udata = udata      # User Defined Data.
        self.points = points    # Leakage points.
        return


# Parse a trs file header.
#   TRSHEADER = |Id(1)|Len(4)|Val(Len)|
def ParseTrsHeaser(trsfd):
    hid = int.from_bytes(trsfd.read(1), byteorder="little")
    hlen = int.from_bytes(trsfd.read(1), byteorder="little")
    hval = trsfd.read(hlen)
    return (hid, hlen, hval)


# Class of trace set.
class TraceSet:
    # Return all traces.
    def GetTraces(self):
        return [x for x in self.tracemtx]

    # Return a list of all User Defined Data.
    def GetData(self):
        return [x.udata for x in self.tracemtx]

    # Return the matrix of all trace points. Each trace is represented as a row.
    def GetAllPoints(self):
        return numpy.matrix([trace.points for trace in self.tracemtx])

    # Return a vector of time points t.
    def GetPoint(self, t):
        return numpy.array([x.points[t] for x in self.tracemtx])

    # Add a new trace to the list.
    def AddTrace(self, trace):
        self.tracemtx.append(trace)
        return

    # Add a set of traces to the list.
    def AddTraceSet(self, traceset):
        for i in trace:
            self.tracemtx.append(i)
        return

    # Print out all taces in the list.
    def Print(self):
        for trace in self.tracemtx:
            print("{}:\t {}".format(hex(trace.udata)), traec.points)
        return

    # Returns a random subset of n traces and optionally its residuel.
    def RandomSubset(self, n, residuel=None):
        rndset = TraceSet()  # The new random trace set.
        rndnums = list(range(len(self.tracemtx)))
        random.shuffle(rndnums)
        n = min(n, len(rndnums))

        for i in range(n):
            # Select a random trace and add it to the new trace set.
            rndset.AddTrace(self.tracemtx[rndnums[i]])

        return rndset

    # Match points specified by f using CPA with HW.
    # The hypothetical value hv[i] is given by function F which is defined as:
    #       hv[i] = F(udata[i])
    #   or with the optional args[i]:
    #       hv[i] = F(udata[i], args[i])
    def CpaMatch(self, F, args=None):
        hvvec = list()
        cors = list()

        # Compute the hypothetical value vector.
        for i in range(len(self.tracemtx)):
            if args == None:
                hvvec.append(F(self.tracemtx[i].udata))
            else:
                hvvec.append(F(self.tracemtx[i].udata, args[i]))

        # Find the point with maximum correlation.
        hvvec = numpy.array(hvvec)
        tracelen = len(self.tracemtx[0].points)
        t = 0
        maxcor = 0
        for i in range(tracelen):
            cor = scipy.stats.pearsonr(hvvec, self.GetPoint(i))[0]
            cors.append(cor)
            if abs(cor) > maxcor:
                maxcor = abs(cor)
                t = i

        return (t, cors)

    # Dump the trace set as a pickle object specified by fp.
    def Dump(self, fp):
        pickle.dump(self, fp)
        return

    # Dump traces into matlab data.
    def DumpToMat(self, filename, udataname, tracename):
        udata = [[numpy.uint8(i) for i in j] for j in self.GetData()]
        scipy.io.savemat(
            filename, {udataname: udata, tracename: self.GetAllPoints()})
        return

    # Print trs header information.
    def PrintHeader(self):
        for key in self.headers:
            print("#{}\t: {}".format(key, self.headers[key]))
        return

    # TraceSet initialiser.
    def __init__(self, trsfile=None, start=0, end=float('inf'), showprogress=True):
        self.headers = dict()
        self.tracemtx = list()
        self.start = start

        # Null trace set.
        if trsfile == None:
            return

        trsfd = open(trsfile, "rb")

        # Parse the header.
        # TRS Format:
        # ---------------------
        # |NT|NS|SC|DS|OPTs|TB|
        # ---------------------
        #   NT(0x41): Number of traces.
        #   NS(0x42): Number of points in each trace.
        #   SC(0x43): Representation for each point.
        #   DS(0x44): Length of user defined data.
        #   TB(0x45): End of header.
        #   OPTs    : Unsupported optional headers.
        try:
            while True:
                (hid, hlen, hval) = ParseTrsHeaser(trsfd)
                if hid == 0x41:  # NT
                    self.headers['NT'] = int.from_bytes(
                        hval, byteorder="little", signed=False)
                elif hid == 0x42:  # NS
                    self.headers['NS'] = int.from_bytes(
                        hval, byteorder="little", signed=False)
                elif hid == 0x43:  # SC
                    self.headers['SC'] = int.from_bytes(
                        hval, byteorder="little", signed=False)
                elif hid == 0x44:  # DS
                    self.headers['DS'] = int.from_bytes(
                        hval, byteorder="little", signed=False)
                elif hid == 0x5F:  # TB
                    break
                else:  # Unsupported optional headers.
                    self.headers[hex(hid)] = hval
                    continue

            # Read traces.
            prc = self.headers['SC'] & 0x0F
            fileIds = range(self.headers['NT'])
            if showprogress:
                fileIds = tqdm(fileIds)

            for i in fileIds:
                # Read User Defined Data
                udata = trsfd.read(self.headers['DS'])

                # Skip points before start.
                trsfd.read(start * prc)

                # Read a trace.
                points = list()
                self.end = int(min(self.headers['NS'], end))
                for j in range(self.start, self.end):
                    measure = trsfd.read(prc)
                    # Interpret the measurement as integer.
                    if self.headers['SC'] & 0x10 == 0:
                        points.append(int.from_bytes(
                            measure, byteorder='little', signed=True))
                    else:   # Interpret the measurement as IEEE 754 float.
                        points.append(struct.unpack('f', measure)[0])

                # Skip points after end.
                trsfd.read(prc * (self.headers['NS'] - self.end))

                # Add the new trace into trace set.
                points = numpy.array(points)
                self.AddTrace(Trace(udata, points))

        finally:
            trsfd.close()

        return


# Load trace set from fp.
def LoadTraceSet(fp):
    return pickle.load(fp)
