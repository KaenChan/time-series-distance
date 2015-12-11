time series distance
---------------------

now support
- dynamic time warping(dtw)
- multivariable dtw
- lb_keogh


demo
====

```
$ python demo_dtw.py
dtw
dtw_c ts1 vs ts2
        56.3702652789   0.03 s
dtw_c ts1 vs ts3
        366.425608996   0.03 s
dtw_py ts1 vs ts3
        366.425608996   7.39 s
```

```
$ python demo_lb_keogh.py
dtw_c ts1 vs ts2
        106.634838282   0.29 s
lb_keogh_c ts1 vs ts2
        116.810789315   0.05 s
lb_keogh_c ts1 vs ts3
        617.903592381   0.05 s
lb_keogh_py ts1 vs ts3
        617.906559519   0.80 s
```

