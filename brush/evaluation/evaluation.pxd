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

cdef extern from "evaluation.h" namespace "FT::Eval":
    cdef cppclass Evaluation:

        Evaluation(string) except +
                    
        void set_score(string)

        void fitness(vector[Individual] &,
                     const Data &, 
                     MatrixXf &, 
                     const Parameters &, 
                     bool,
                     bool)
          
        #TODO CLabels being used here need to remove
        void assign_fit(Individual &, MatrixXf &, const shared_ptr[CLabels] &, 
                        const VectorXf &, const Parameters &, bool)      
