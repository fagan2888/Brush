# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from eigency.core cimport *

from brush.population.population cimport CPopulation
from brush.params.params cimport CParameters

cdef extern from "src/selection.h" namespace "FT::Sel":
    cdef cppclass CSelection:

        CSelection() except +
        
        CSelection(string, bool) except +

        string get_type()

        vector[size_t] select(CPopulation &,
                              const CParameters &)

        vector[size_t] survive(CPopulation &,
                               const CParameters &)
                               
                               
cdef cppclass Selection:
    CSelection sel

