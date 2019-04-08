# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = rnd.cc

from brush.util.rnd cimport CRnd

cdef class Rnd:
    cdef CRnd r

    def __cinit__(self):
        self.r = Rnd::initRand()
        
    def __dealloc__(self):
        Rnd::destroy()
        
    def set_seed(self, int seed):
        self.r.set_seed(seed)
