# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = nodevector.cc

from libcpp.vector cimport vector
from libcpp.string cimport string


from brush.program.program cimport CNodeVector
from brush.program.node import Node

cdef class Program:
    cdef CNodeVector nodevec
    
    def __cinit__(self):
        self.nodevec = CNodeVector()
    
    def allocate(self, Program other):
        self.nodevec = CNodeVector(other.nodevec)
        
    def program_str(self):
        return self.nodevec.program_str()
            
    def roots(self):
        return self.nodevec.roots()
        
    def subtree(self, size_t i, char otype):
        return self.nodevec.subtree(i, otype)
        
    cdef set_weights(self, vector[vector[float]] &weights):
        self.nodevec.set_weights(weights)
        
    cdef get_weights(self):
        return self.nodevec.get_weights()
         
    def is_valid_program(self, unsigned num_features, vector[string] longitudinalMap):
        return self.nodevec.is_valid_program(num_features, longitudinalMap)
        
    cdef make_tree(self, Program functions, 
                   Program terminals, int max_d,  
                   const vector[float] &term_weights, char otype, const vector[char] &term_types):
        self.nodevec.make_tree(functions.nodevec, terminals.nodevec, max_d, term_weights, otype, term_types)

    cdef make_program(self, Program functions, 
                      Program terminals, int max_d, 
                      const vector[float] &term_weights, int dim, char otype, 
                      vector[string] longitudinalMap, const vector[char] &term_types):
        self.nodevec.make_program(functions.nodevec, terminals.nodevec, max_d, term_weights, dim, otype, longitudinalMap, term_types)
    
