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
