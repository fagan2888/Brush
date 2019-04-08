# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

cdef extern from "rnd.h" namespace "FT::Util":
    cdef cppclass CRnd:
        
        @staticmethod
        CRnd* initRand()
        
        @staticmethod
        void destroy()

        void set_seed(int)
