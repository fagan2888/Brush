# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = nodevector.cc

from brush.program.program cimport CNodeVector

cdef class Program:
    cdef CNodeVector nodevec
    
    def __cinit__(self):
        self.nodevec = CNodeVector()
    
    cdef __cinit__(self, const NodeVector& other):
        self.nodevec = CNodeVector(other.nodevec)
        
    cdef __cinit__(self, NodeVector && other):
        self.nodevec = CNodeVector(&& other.nodevec)
        
    cdef NodeVector& operator=(self, NodeVector const &other):
        return self.nodevec.=(other.nodevec)
        
    def NodeVector& operator=(self, NodeVector && other):
        return self.nodevec.=(&& other.nodevec)
        
    def vector[Node*] get_data(self, int start, int end):
        return self.nodevec.get_data(start, end)
        
    def vector[size_t] roots(self) const:
        return self.nodevec.roots()
        
    def size_t subtree(self, size_t i, char otype) const:
        return self.nodevec.subtree(i, otype)
        
    cdef void set_weights(self, vector[vector[float]] &weights):
        self.nodevec.set_weights(weights)
        
    def vector[vector[float]] get_weights(self):
        return self.nodevec.get_weights()
         
    def bool is_valid_program(self, unsigned num_features, vector[string] longitudinalMap):
        return self.nodevec.is_valid_program(num_features, longitudinalMap)
        
    cdef void make_tree(self, const NodeVector& functions, 
                        const NodeVector& terminals, int max_d,  
                        const vector[float] &term_weights, char otype, const vector[char] &term_types)
        self.nodevec.make_tree(functions.nodevec, terminals.nodevec, max_dm term_weights, otype, term_types)

    cdef void make_program(self, const NodeVector &functions, 
                           const NodeVector& terminals, int max_d, 
                           const vector[float] &term_weights, int dim, char otype, 
                           vector[string] longitudinalMap, const vector[char] &term_types)
        self.nodevec.make_program(functions.nodevec, terminals.nodevec, max_d, term_weights, dim, otype, longitudinalMap, term_types)
    
