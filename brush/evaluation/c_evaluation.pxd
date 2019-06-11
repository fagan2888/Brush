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
from brush.population.population cimport CPopulation

cdef extern from "src/evaluation.h" namespace "FT::Eval":
    cdef cppclass CEvaluation:
    
        CEvaluation() except +

        CEvaluation(string) except +
                    
        void set_score(string)

        void fitness(CPopulation &,
                     const CData &, 
                     const CParameters &, 
                     bool,
                     bool)
          
        void assign_fit(CIndividual &, const VectorXf &, 
                        const VectorXf &, const CParameters &, bool) 
                        

# cdef class Evaluation:
#     CEvaluation evalobj     
