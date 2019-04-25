# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.memory cimport shared_ptr
from libcpp cimport bool
from eigency.core cimport *

from brush.data.data cimport CData
from brush.params.params cimport CParameters


cdef extern from "src/individual.h" namespace "FT::Pop":
    cdef cppclass CIndividual:
        CIndividual() except +

        VectorXf out(const CData &, const CParameters &, bool)
                           
        VectorXf fit(const CData &, const CParameters &, bool &)
        
        VectorXf predict(const CData &, const CParameters &)

        string get_eqn()

        vector[string] get_features()

        void set_rank(unsigned)

        int get_n_params()

        unsigned int get_dim()

        unsigned int complexity()
        
        unsigned int get_complexity()

        void set_id(unsigned i)

        vector[float] get_p() const

        void set_p(const vector[float] &, const float &,
                   const bool)
                   

cdef cppclass Individual:
    CIndividual ind

