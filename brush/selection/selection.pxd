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

cdef extern from "selection.h" namespace "FT::Sel":
    cdef struct Selection:

        Selection(string, bool) except +

        string get_type()

        vector[size_t] select(Population &,
                              const MatrixXf &,
                              const Parameters &)

        vector[size_t] survive(Population &,
                               const MatrixXf &,
                               const Parameters &)
