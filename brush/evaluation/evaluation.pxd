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
from ..population import Individual
from brush.population cimport CIndividual

cdef extern from "evaluation.h" namespace "FT::Eval":
    cdef cppclass CEvaluation:

        CEvaluation(string) except +
                    
        void set_score(string)

        void fitness(vector[CIndividual] &,
                     const Data &, 
                     MatrixXf &, 
                     const Parameters &, 
                     bool,
                     bool)
          
        #TODO CLabels being used here need to remove
        void assign_fit(CIndividual &, MatrixXf &, const shared_ptr[CLabels] &, 
                        const VectorXf &, const Parameters &, bool)      
