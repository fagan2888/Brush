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
from libcpp.memory cimport shared_ptr
from libcpp cimport bool
from eigency.core cimport *

cdef extern from "individual.h" namespace "FT::Pop":
    cdef cppclass Individual:
        Individual() except +

        MatrixXf out(const Data &, const Parameters &, bool)

        MatrixXf out_trace(const Data &,
                           const Parameters &,
                           vector[Trace] &)

        string get_eqn()

        vector[string] get_features()

        string program_str() const

        void set_rank(unsigned)

        int get_n_params()

        unsigned int get_dim()

        unsigned int complexity()

        void set_id(unsigned i);

        void set_parents(const vector[Individual] &)

        void set_parents(const vector[int] &)

        vector[float] get_p() const

        float get_p(const size_t, bool) const

        vector[float] get_p(const vector[size_t] &, bool) const

        void set_p(const vector[float] &, const float &,
                   const bool)

