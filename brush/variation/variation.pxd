# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++

import ctypes
from libcpp.vector cimport vector

cdef extern from "variation.h" namespace "FT::Vary":
    cdef cppclass CVariation:
        CVariation(float) except +

        void set_cross_rate(float)

        float get_cross_rate()

        void vary(CPopulation &,
                  const vector[size_t] &,
                  const CParameters &,
                  const CData &)
