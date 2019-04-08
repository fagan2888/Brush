# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool

from brush.program.node cimport CNode

cdef extern from "nodevector.h" namespace "FT::Pop::Op":
    cdef cppclass CNodeVector:

        CNodeVector() except +
            
        CNodeVector(const CNodeVector &) except +
        
        #TODO verify what this means
        CNodeVector(CNodeVector && other) except +
        
        CNodeVector& operator=(const CNodeVector &)
        
        #TODO check if required
        CNodeVector& operator=(CNodeVector &&)
        
        vector[CNode*] get_data(int, int)

        vector[size_t] roots() const;

        size_t subtree(size_t, char) const
        
        void set_weights(vector[vector[float]] &)
        
        vector[vector[float]] get_weights()
        
        bool is_valid_program(unsigned, vector[string])

        void make_tree(const CNodeVector &,  
                       const CNodeVector &, int,  
                       const vector[float] &, char, const vector[char] &)

        void make_program(const CNodeVector & , 
                          const CNodeVector &, int, 
                          const vector[float] &, int, char, 
                          vector[string], const vector[char] &)
