### REFCNT.H
#### A reference-counting garbage collector

```refcnt.h``` aims to be as usable as malloc/free, such that a (memory-safe) program using malloc/free works just as well if every instance of 'malloc' is replaced with 'refcnt_new' and every instance of 'free' is replaced with 'refcnt_unref'. 
