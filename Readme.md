time series distance
---------------------

now support
- dynamic time warping (DTW)
- multivariable DTW
- lb_keogh

todo
- multivariable lb_keogh
- edit distance on real (EDR)


## demo

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
        101.577439205   0.30 s
lb_keogh_c ts1 vs ts2
        28.1342295012   0.05 s
lb_keogh_c ts1 vs ts3
        4697.5134284    0.05 s
lb_keogh_py ts1 vs ts3
        4697.51370379   0.79 s
```

