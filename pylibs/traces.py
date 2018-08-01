import glob
import numpy
import scipy
import scipy.stats
import scipy.io
import random
import pickle
import sys
import struct
import copy
from tqdm import tqdm


# Parse a trs file header.
#   TRSHEADER = |Id(1)|Len(4)|Val(Len)|
def ParseTrsHeader(trsfd):
    hid = int.from_bytes(trsfd.read(1), byteorder="little")
    hlen = int.from_bytes(trsfd.read(1), byteorder="little")
    hval = trsfd.read(hlen)
    return (hid,  hlen, hval)


# Get only header information from a trs file.
def ReadTrsHeader(trsfd):
    headers = dict()
    while True:
        (hid, hlen, hval) = ParseTrsHeader(trsfd)
        if hid == 0x41:  # NT
            headers['NT'] = int.from_bytes(
                hval, byteorder="little", signed=False)
        elif hid == 0x42:  # NS
            headers['NS'] = int.from_bytes(
                hval, byteorder="little", signed=False)
        elif hid == 0x43:  # SC
            headers['SC'] = int.from_bytes(
                hval, byteorder="little", signed=False)
        elif hid == 0x44:  # DS
            headers['DS'] = int.from_bytes(
                hval, byteorder="little", signed=False)
        elif hid == 0x5F:  # TB
            break
        else:  # Unsupported optional headers.
            headers[hex(hid)] = hval
            continue

    return headers


# Copy TraceSet Metadata. The traces object is ignored.
def CopyTraceSetMetadata(src, dst):
    for k in dst.__dict__:
        if k != 'traces':
            dst.__dict__[k] = copy.deepcopy(src.__dict__[k])
    return


# Class of a single trace.
class Trace:
    def __init__(self, udata=None, points=None):
        self.udata = udata      # User Defined Data.
        self.points = points    # Leakage points.
        return


# Class of trace set.
class TraceSet:
    # TraceSet initialiser.
    def __init__(self, trsfile=None, start=0, end=float('inf'), showprogress=True, showheader=False):
        self.version = 1        # TraceSet class Version.
        self.headers = dict()   # TRS headers.
        self.start = start      # Trace starting point.
        self.end = float('inf')  # Trace end point.
        self.traces = list()  # List of traces.

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
                (hid, hlen, hval) = ParseTrsHeader(trsfd)
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

            if showheader:
                self.PrintHeader()

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
                self.traces.append(Trace(udata, points))

        finally:
            trsfd.close()

        return

   # Return a list of all traces.
    def GetTraces(self):
        return [x for x in self.traces]

        # Return a list of all User Defined Data.
    def GetData(self):
        return [x.udata for x in self.traces]

    # Return the matrix of all trace points. Each trace is represented as a row.
    def GetAllPoints(self):
        return numpy.matrix([trace.points for trace in self.traces])

    # Return a vector of time points t.
    def GetPoint(self, t):
        return numpy.array([x.points[t] for x in self.traces])

    # Return the number of traces.
    def NTrace(self):
        return self.headers['NT']

    # Return the length of traces.
    def LTrace(self):
        return self.end - self.start


    # For backward compatibility.
    def Len(self):
        return NTrace(self)

    # Add a new trace to the list.
    def AddTrace(self, trace):
        self.traces.append(trace)
        self.headers['NT'] += 1
        return

    # Add a set of traces to the list.
    def AddTraceSet(self, traceset):
        for t in traceset.GetTraces():
            self.AddTrace(t)

        return

    # Fork an empty copy of this trace set with all meta data except headers['NT'].
    def Fork(self):
        child = TraceSet()
        CopyTraceSetMetadata(self, child)
        child.headers['NT'] = 0

        return child

    # Returns a random subset of n traces and optionally its residuel.
    def RandomSubset(self, n, withResiduel = False):
        rndset = self.Fork()  # The new random trace set.
        rndnums = range(len(self.traces))
        n = min(n, len(rndnums))
        rndnums = numpy.random.choice(rndnums, n, replace = False)

        for i in range(n):
            # Select a random trace and add it to the new trace set.
            rndset.AddTrace(self.traces[rndnums[i]])

        if withResiduel:
            residule = self.Fork()
            for i in range(n, len(self.traces)):
                residuel.AddTrace(self.traces[rndnums[i]])

            return (rndset, residule)


        else:
            return rndset

    # Match points specified by f using CPA with HW.
    # The hypothetical value hv[i] is given by function F which is defined as:
    #       hv[i] = F(udata[i])
    #   or with the optional args[i]:
    #       hv[i] = F(udata[i], args[i])
    def CpaMatch(self, F, args=None, start=0, end=float('inf')):
        hvvec = list()
        cors = list()

        # Compute the hypothetical value vector.
        for i in range(len(self.traces)):
            if args == None:
                hvvec.append(F(self.traces[i].udata))
            else:
                hvvec.append(F(self.traces[i].udata, args[i]))

        # Find the point with maximum correlation.
        hvvec = numpy.array(hvvec)
        end  = int(min(self.end - self.start, end))
        t = 0
        maxcor = 0
        for i in range(start, end):
            cor = scipy.stats.pearsonr(hvvec, self.GetPoint(i))[0]
            cors.append(cor)
            if abs(cor) > maxcor:
                maxcor = abs(cor)
                t = i

        return (t - start, cors)

    # Dump the trace set as a pickle object specified by fp.
    def Dump(self, fp):
        pickle.dump(self.__dict__, fp)
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

    # Print out all taces in the list.
    def Print(self):
        self.PrintHeader()
        for trace in self.traces:
            print("{}:\t {}".format(hex(trace.udata)), traec.points)
        return



# Load trace set from fp.
def LoadTraceSet(fp):
    tsobj = TraceSet()
    tsdata = pickle.load(fp)
    tsobj.__dict__ = tsdata
    return tsobj
