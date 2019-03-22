# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.memory cimport unique_ptr
from libcpp.map cimport map
from libcpp.utility cimport pair
from eigency.core cimport *

cdef extern from "params.h" namespace "FT":
    cdef struct Parameters:        
        Parameters(int, int, string, bool, int, 
                   char, int, string, float, float, 
                   unsigned int, unsigned int, bool, string, 
                   bool, float, float, string, string, bool, 
                   int, float, int, bool, int, bool, bool, 
                   bool, bool) except +
        
        
        void init()
      
        void set_current_gen(int)
        
        void set_scorer(string)
        
        void set_term_weights(const vector<float>&)
        
        unique_ptr[Node] createNode(string, float, bool, 
                                    size_t, string)
        
        void set_functions(string)
        
        void updateSize()
        
        void set_max_depth(unsigned int)
        
        void set_max_dim(unsigned int)
        
        void set_terminals(int,
                           map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ])

        void set_feature_names(string)

        void set_objectives(string)
        
        void set_verbosity(int)

        void set_otype(char)
        
        void set_ttypes()

        void set_otypes(bool)
        
        void set_classes(VectorXf&)    
        
        void set_sample_weights(VectorXf& y)
