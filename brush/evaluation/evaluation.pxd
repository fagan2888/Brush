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


from brush.population.individual cimport CIndividual
from brush.data.data cimport CData
from brush.params.params cimport CParameters

cdef extern from "evaluation.h" namespace "FT::Eval":
    cdef cppclass CEvaluation:
    
        CEvaluation() except +

        CEvaluation(string) except +
                    
        void set_score(string)

        void fitness(vector[CIndividual] &,
                     const CData &, 
                     MatrixXf &, 
                     const CParameters &, 
                     bool,
                     bool)
          
        #TODO CLabels being used here need to remove
        void assign_fit(CIndividual &, MatrixXf &, const VectorXf &, 
                        const VectorXf &, const CParameters &, bool)      
