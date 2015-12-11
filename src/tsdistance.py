#! /usr/bin/env python2
from ctypes import cdll, c_double, c_int, c_char_p, byref, Structure
from ctypes import *
import ctypes
import os
import numpy as np
import sys

__all__ = ['dtw_c', 'dtw_py', 'lb_keogh_c', 'lb_keogh_py']

def load_lib():
    curr_path = os.path.dirname(os.path.abspath(os.path.expanduser(__file__)))
    dll_path = [curr_path]
    dll_path = [os.path.join(p, 'libdtw_c.so') for p in dll_path]
    lib_path = [p for p in dll_path if os.path.exists(p) and os.path.isfile(p)]
    if len(lib_path) == 0:
        raise ("dtw_c library not found. Did you run ../make?")

    # print lib_path[0]

    lib = ctypes.cdll.LoadLibrary(lib_path[0])
    return lib

l_dtw_c = load_lib()

def trans(s1,s2,w):
    n1 = len(s1)
    if type(s1[0])==list or type(s1[0])==np.ndarray:
        k=len(s1[0])
        array_of_n_doubles1 = c_double*(n1*k)
        ts1 = array_of_n_doubles1()
        for ki in range(k):
            for i in range(n1):
                idx = ki*n1+i
                ts1[idx] = s1[i][ki]
    else:
        k=1
        array_of_n_doubles1 = c_double*n1
        ts1 = array_of_n_doubles1()
        for i in range(n1):
            ts1[i] = s1[i]

    n2 = len(s2)
    if type(s2[0])==list or type(s2[0])==np.ndarray:
        k2=len(s2[0])
        array_of_n_doubles2 = c_double*(n2*k)
        ts2 = array_of_n_doubles2()
        for ki in range(k):
            for i in range(n2):
                idx = ki*n2+i
                ts2[idx] = s2[i][ki]
    else:
        k2=1
        array_of_n_doubles2 = c_double*n2
        ts2 = array_of_n_doubles2()
        for i in range(n2):
            ts2[i] = s2[i]
    assert k==k2
    return ts1,ts2,n1,n2,k

def dtw_c(s1,s2,w=20):
    ts1,ts2,n1,n2,k = trans(s1,s2,w)
    l_dtw_c.dtw_c.argtypes = [c_void_p, c_void_p, c_int, c_int, c_int, c_int]
    l_dtw_c.dtw_c.restype = c_double
    return l_dtw_c.dtw_c(ts1, ts2, w, n1, n2, k)


def lb_keogh_c(s1,s2,w=20):
    ts1,ts2,n1,n2,k = trans(s1,s2,w)
    if k!=1:
        raise Exception('The lb_keogh do not support multivariable')
    l_dtw_c.lb_keogh_c.argtypes = [c_void_p, c_void_p, c_int, c_int, c_int, c_int]
    l_dtw_c.lb_keogh_c.restype = c_double
    return l_dtw_c.lb_keogh_c(ts1, ts2, w, n1, n2, k)


def dtw_py(s1,s2,w=20):
    '''
    Calculates dynamic time warping Euclidean distance between two
    sequences. Option to enforce locality constraint for window w.
    '''
    DTW={}

    if w!=-1:
        w = max(w, abs(len(s1)-len(s2)))

    for i in range(-1,len(s1)):
        for j in range(-1,len(s2)):
            DTW[(i, j)] = float('inf')

    DTW[(-1, -1)] = 0

    for i in range(len(s1)):
        if w==-1:
            j1=0
            j2=len(s2)
        else:
            j1=max(0, i-w)
            j2=min(len(s2), i+w)
        for j in range(j1,j2+1):
            dist = np.sum((s1[i]-s2[j])**2)
            DTW[(i, j)] = dist + min(DTW[(i-1, j)],DTW[(i, j-1)], DTW[(i-1, j-1)])

    # view matrix D 
    # print
    # for i in range(-1,len(s1)):
    #     for j in range(-1,len(s2)):
    #         print("%8.2f " % DTW[(i, j)]),
    #     print

    return np.sqrt(DTW[len(s1)-1, len(s2)-1])


def lb_keogh_py(s1,s2,w=20):
    '''
    calculates lb_keough lower bound to dynamic time warping. linear
    complexity compared to quadratic complexity of dtw.
    '''
    if type(s1[0])==list or type(s1[0])==np.ndarray:
        k=len(s1[0])
        if k!=1:
            raise Exception('The lb_keogh do not support multivariable')

    n1 = len(s1)
    n2 = len(s2)
    n = min(n1,n2)
    if w>=n:
        w=n-1

    lb_sum=0
    # print
    # print 'w', w
    for idx,s in enumerate(s1):
        if idx>=n: break

        j1 = max(idx-w, 0)
        j2 = min(idx+w, n)
        lower_bound=np.min(s2[j1:j2])
        upper_bound=np.max(s2[j1:j2])

        if s>upper_bound:
            lb_sum=lb_sum+np.sum((s-upper_bound)**2)
        elif s<lower_bound:
            lb_sum=lb_sum+np.sum((s-lower_bound)**2)

        # print lower_bound, upper_bound, lb_sum

    return np.sqrt(lb_sum)


def __main__():
    import numpy as np
    import pandas as pd
    x=np.linspace(0,50,1000)
    ts1=pd.Series(3.1*np.sin(x/1.5)+3.5)
    ts2=pd.Series(2.2*np.sin(x/3.5+2.4)+3.2)
    ts3=pd.Series(0.04*x+3.0)

    #use an array
    from time import time
    t1=time()
    print dtw(ts1, ts2, w=10)
    print '%.2f s' % (time()-t1)


    import dtw_py
    t1=time()
    print dtw_py.dtw(ts1, ts2, w=10)
    print '%.2f s' % (time()-t1)
