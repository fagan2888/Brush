# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = rnd.cc

from Rnd cimport Rnd

cdef class PyRnd:
    cdef Rnd r

    def __cinit__(self):
        self.r = Rnd::initRand()
        
    def __dealloc__(self):
        Rnd::destroy()

    def set_seed(self, int seed):
        self.r.set_seed(seed)
        
    def rnd_int(self, int lowerLimit, int upperLimit):
        return self.r.rnd_int(lowerLimit, upperLimit)

    def rnd_flt(self, float lower, float upper):
        return self.r.rnd_flt(lower, upper)
        
    def rnd_dbl(self, float lower, float upper):
        return self.r.rnd_dbl(lower, upper)
        
    def operator()(self, unsigned i):
        return self.r(i)
        
    def operator()(self):
        return self.r()
        
    def void shuffle[RandomAccessIterator] (self, RandomAccessIterator first, RandomAccessIterator last):
        self.r.shuffle(first, last)
        
    def select_randomly[T](self, T start, T end):
        return self.r.select_randomly(start, end)
        
    cdef random_choice[T](self, const vector[T] &v):
        return self.r.random_choice(v)
        
    cdef random_choice[T][D](self, const vector[T] &v, const vector[D] &w):
        return self.r.random_choice(v, w)
        
    def gasdev(self):
        return self.r.gasdev()
        
