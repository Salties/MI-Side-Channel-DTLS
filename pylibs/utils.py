#!/usr/bin/python3

import sys
import math
import scipy.stats


# Compute the Hamming Weight of x.
try:
    import gmpy
    def HW(x):
        return gmpy.popcount(int(x))

except ImportError:
    def HW(x):
        return bin(x).count('1')


# Compute the Hamming Distance between x and y.
def HD(x,y):
    return HW(x^y)


# Sort elements in L by dimension d, defualt by ascending.
def SortDim(L, d = 0, descend = False):
    K = sorted([(i[d], i) for i in L], reverse = descend)
    Ls = [i[1] for i in K]
    return Ls


# Right rotate x by i bits for N-bits register.
def rotr(x, i, N):
    i = i % N
    return (2**N - 1) & ((x << (N - i)) | (x >> i))


# Computes the Shannon's Entropy for the list L.
def ShannonEntropy(L):
    # Compute the empirical probability for each element in L
    X = dict()
    for i in L:
        if i not in X.keys():
            X[i] = 1
        else:
            X[i] += 1

    N = len(L)
    P = [float(X[i] / len(L)) for i in X]

    H = -sum([i * math.log(i, 2) for i in P])

    # H = scipy.stats.entropy(P)

    return H
