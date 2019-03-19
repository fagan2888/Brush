# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "nodevector.h" namespace "FT::Pop::Op":
    cdef struct NodeVector:

        NodeVector() except +
            
        NodeVector(const NodeVector &) except +
        
        #TODO verify what this means
        NodeVector(NodeVector && other) except +
        
        NodeVector& operator=(NodeVector const &)
        
        NodeVector& operator=(NodeVector &&)
        
        vector[Node*] get_data(int, int)

        vector[size_t] roots() const;

        size_t subtree(size_t, char) const
        
        void set_weights(vector[vector[float]] &)
        
        vector[vector[float]] get_weights()
        
        bool is_valid_program(unsigned, vector[string])

        void make_tree(const NodeVector &,  
                       const NodeVector &, int,  
                       const vector[float] &, char, const vector[char] &)

        void make_program(const NodeVector & , 
                          const NodeVector &, int, 
                          const vector[float] &, int, char, 
                          vector[string], const vector[char] &)
