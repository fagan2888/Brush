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

cdef extern from "src/params.h" namespace "FT":
    cdef cppclass CParameters:    
        CParameters() except +
            
        CParameters(int pop_size, int gens, bool classification, 
                    int max_stall, char output_type, int verbosity, string functions, 
                    float xo_rate, unsigned int max_depth, unsigned max_dim, 
                    string objectives, bool shuffle, float split, float feedback, 
                    string scorer, string feature_names, bool backprop, int iters,
                    float learning_rate, int batch_size, bool hill_climb,
                    int max_time, bool use_batch) except +
        
        void init()
      
        void set_current_gen(int)
        
        void set_scorer(string)
        
        void set_term_weights(const vector[float]&)
        
        # unique_ptr[Node] createNode(string, float, bool, 
        #                             size_t, string)
        
        void set_functions(string)
        
        # void updateSize()
        
        void set_max_depth(unsigned int)
        
        void set_terminals(int)

        void set_feature_names(string)

        void set_objectives(string)
        
        void set_verbosity(int)

        void set_otype(char)
        
        void set_ttypes()

        void set_otypes(bool)
        
        void set_classes(VectorXf&)    
        
        void set_sample_weights(VectorXf& y)

        bool classification, shuffle, use_batch
        char otype
        int pop_size, current_gen, gens, max_depth, max_dim, max_stall, verbosity
        float split
        string scorer
        

cdef class Parameters:
    cdef CParameters c_params
    # cdef set_terminals(self, int nf,
    #                   map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] z)
    cdef set_classes(self, VectorXf& y)
    cdef set_sample_weights(self, VectorXf& y)
    # cdef public int current_gen, verbosity, max_depth, max_dim, max_stall
    # cdef public bool classification     
    # cdef public char otype
