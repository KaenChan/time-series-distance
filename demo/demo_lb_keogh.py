#! /usr/bin/env python2
import numpy as np
import pandas as pd
from tsdistance import dtw_c
from tsdistance import lb_keogh_c
from tsdistance import lb_keogh_py
from time import time

x=np.linspace(0,500,4000)
ts1=pd.Series(3.1*np.sin(x/1.5)+3.5)
x=np.linspace(0,500,3000)
ts2=pd.Series(2.2*np.sin(x/3.5+2.4)+3.2)
ts3=pd.Series(0.04*x+3.0)

t1=time()
print 'dtw_c ts1 vs ts2'
print '\t', dtw_c(ts1, ts2, w=10),
print '\t%.2f s' % (time()-t1)

t1=time()
print 'lb_keogh_c ts1 vs ts2'
print '\t', lb_keogh_c(ts1, ts2, w=10),
print '\t%.2f s' % (time()-t1)

t1=time()
print 'lb_keogh_c ts1 vs ts3'
print '\t', lb_keogh_c(ts1, ts3, w=10),
print '\t%.2f s' % (time()-t1)

t1=time()
print 'lb_keogh_py ts1 vs ts3'
print '\t',lb_keogh_py(ts1, ts3, w=10),
print '\t%.2f s' % (time()-t1)
print

