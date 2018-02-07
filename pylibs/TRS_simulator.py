#!/usr/bin/python3

import TRS_TraceSet

import numpy
import random

# Generate a TRS file with simulated traces.
def GenerateSimTrs(filename, #Output trs file name.
        SimFunc, # Simulation target function defined by user.
        nt=1,   # Number of traces.
        ns=1,   # Number of points in each trace.
        udatalen=2, # Length of user defined data.
        upt=None):   # (Optional) User defined plaintext.
    
    # Initlise and create a TRS file.
    trsfile = TRS_TraceSet.TRS_TraceSet(filename)
    trsfile.write_header_Sim(nt, ns, udatalen)

    # Write traces to the TRS file.
    for i in range(nt):
        if upt == None: # (Default) Use random plaintext.
            pt = random.getrandbits(4*udatalen).to_bytes(int(udatalen/2), 'little')
            (ct, points) = SimFunc(pt)
        else:   # Use plaintext specified by user.
            pt = bytes(upt[i])
            (ct, points) = SimFunc(upt[i])

        # Append the trace into TRS file.
        trsfile.write_trace(pt, ct, points)

    return
