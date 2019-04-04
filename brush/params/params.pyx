# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = params.cc

from brush.params.params cimport CParameters
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.map cimport map
from libcpp.utility cimport pair
from eigency.core cimport *

cdef class Parameters:
    cdef CParameters params
    
    def __cinit__(self, int pop_size, int gens, string ml, bool classification, 
                    int max_stall, char ot, int verbosity, string fs, float cr,
                    float root_xor, unsigned int max_depth, unsigned int max_dim,
                    bool constant, string obj, bool sh, float sp, float fb, string sc,
                    string fn, bool bckprp, int iters, float lr, int bs, bool hclimb,
                    int maxt, bool useb, bool res_xo, bool stg_xo, 
                    bool sftmx):
                
        # cdef char ot_char
        # if ( len(ot) == 0):
        #     ot_char = 'a' #Defaut Value
        # else:
        #     ot_char = ord(ot)
        self.params = CParameters(pop_size, gens, ml, classification, max_stall, 
                                  ot, verbosity, fs, cr, root_xor, 
                                  max_depth, max_dim, constant, obj, 
                                  sh, sp, fb, sc, fn, bckprp, 
                                  iters, lr, bs, hclimb, maxt, useb, res_xo, 
                                  stg_xo, sftmx)
        
    def init(self):
        self.params.init()
  
    def set_current_gen(self, int g):
        self.params.set_current_gen(g)
    
    def set_scorer(self, string sc):
        self.params.set_scorer(sc)
    
    def set_term_weights(self, const vector[float]& w):
        self.params.set_term_weights(w)
        
    # def createNode(self, string name, float d_val, bool b_val, 
    #                                  size_t loc, string name):
    #     return self.params.createNode(name, d_val, b_val, loc, name)
    
    def set_functions(self, string fs):
        self.params.set_functions(fs)
    
    def updateSize(self):
        self.params.updateSize()
    
    def set_max_depth(self, unsigned int max_depth):
        self.params.set_max_depth(max_depth)
    
    def set_max_dim(self, unsigned int max_dim):
        self.params.set_max_dim(max_dim)
    
    def set_terminals(self, int nf,
                      map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] z):
        self.params.set_terminals(nf, z)

    def set_objectives(self, string obj):
        self.params.set_objectives(obj)
    
    def set_verbosity(self, int verbosity):
        self.params.set_verbosity(verbosity)

    def set_otype(self, char ot):
        self.params.set_otype(ot)
    
    def set_ttypes(self):
        self.params.set_ttypes()

    def set_otypes(self, bool terminals_set):
        self.params.set_otypes(terminals_set)
        
    def set_classes(self, VectorXf& y):    
        self.params.set_classes(y)
            
    def set_sample_weights(self, VectorXf& y):
        self.params.set_sample_weights(y)
