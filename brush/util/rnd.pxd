# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# TODO check how inheritance works in cython
# need to inherit selectionoperator class here

import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string

cdef extern from "rnd.h" namespace "FT::Util":
    cdef cppclass Rnd:
        
        @staticmethod
        Rnd* initRand()
        
        @staticmethod
        void destroy()

        void set_seed(int)