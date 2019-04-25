# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++

import ctypes
from libcpp.vector cimport vector

from brush.population.population cimport CPopulation
from brush.params.params cimport CParameters
from brush.data.data cimport CData

cdef extern from "variation.h" namespace "FT::Vary":
    cdef cppclass CVariation:
    
        CVariation() except +
        CVariation(float) except +

        void set_cross_rate(float)

        float get_cross_rate()

        void vary(CPopulation &,
                  const vector[size_t] &,
                  const CParameters &,
                  const CData &)
                  
                  
cdef cppclass Variation:
    CVariation vary_obj
