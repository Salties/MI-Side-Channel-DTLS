#!/usr/bin/python3

import gmpy
import math
import scipy.stats


# Compute the Hamming Weight of x.
def HW(x):
    return gmpy.popcount(x)


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
