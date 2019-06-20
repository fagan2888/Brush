# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++

from brush.params.params cimport CParameters
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.map cimport map
from libcpp.utility cimport pair
from eigency.core cimport *

cdef class Parameters:
    """This is the wrapper class that stores all the parameters for Brush."""
    # cdef CParameters c_params
    
    def __cinit__(self, int pop_size, 
                    int gens, bool classification, 
                    int max_stall, 
                    string output_type, 
                    int verbosity, 
                    string functions, 
                    float xo_rate, 
                    unsigned int max_depth, unsigned int max_dim, 
                    string objectives, bool shuffle, float split, float feedback, 
                    string scorer, string feature_names, bool backprop, int iters,
                    float learning_rate, int batch_size, bool hill_climb,
                    int max_time, bool use_batch):
                  
        # convert output_type to char
        cdef char ot_char
        if ( len(output_type) == 0):
            ot_char = 'a' #Defaut Value
        else:
            ot_char = ord(output_type)

        self.c_params = CParameters(pop_size, gens, classification, max_stall, 
                                  ot_char, verbosity, functions, xo_rate,  
                                  max_depth, max_dim, objectives, 
                                  shuffle, split, feedback, scorer, feature_names, 
                                  backprop, iters, learning_rate, batch_size, 
                                  hill_climb, max_time, use_batch)

    def init(self):
        self.c_params.init()
  
    
    # def set_term_weights(self, const vector[float]& w):
    #     self.c_params.set_term_weights(w)
        
    # def createNode(self, string name, float d_val, bool b_val, 
    #                                  size_t loc, string name):
    #     return self.c_params.createNode(name, d_val, b_val, loc, name)
    
    # def set_functions(self, string fs):
    #     self.c_params.set_functions(fs)
    
    # def set_objectives(self, string obj):
    #     self.c_params.set_objectives(obj)
    
    def set_terminals(self, int nf):
        self.c_params.set_terminals(nf)
    
    # cdef set_terminals(self, int nf,
    #                   map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] z):
    #     self.c_params.set_terminals(nf, z)
    
    cdef set_classes(self, VectorXf& y):    
        self.c_params.set_classes(y)
            
    cdef set_sample_weights(self, VectorXf& y):
        self.c_params.set_sample_weights(y)

    # Attribute access
    @property
    def current_gen(self):
        return self.c_params.current_gen
    @current_gen.setter
    def current_gen(self, current_gen):
        self.c_params.set_current_gen(current_gen)
    
    # Attribute access
    @property
    def gens(self):
        return self.c_params.gens
    @gens.setter
    def gens(self, gens):
        self.c_params.gens = gens 

    @property
    def classification(self):
        return self.c_params.classification
    @classification.setter
    def classification(self, classification):
        self.c_params.classification = classification

    @property
    def max_stall(self):
        return self.c_params.max_stall
    @max_stall.setter
    def max_stall(self, max_stall):
        self.c_params.max_stall = max_stall

    @property
    def otype(self):
        return self.c_params.otype
    @otype.setter
    def otype(self, otype):
        self.c_params.set_otype(otype)
    
    @property
    def verbosity(self):
        return self.c_params.verbosity
    @verbosity.setter
    def verbosity(self, verbosity):
        self.c_params.set_verbosity(verbosity)
    
    @property
    def max_depth(self):
        return self.c_params.max_depth
    @max_depth.setter
    def max_depth(self, max_depth):
        self.c_params.set_max_depth(max_depth)
    
    @property
    def shuffle(self):
        return self.c_params.shuffle
    @shuffle.setter
    def shuffle(self, shuffle):
        self.c_params.shuffle = shuffle

    @property
    def split(self):
        return self.c_params.split
    @split.setter
    def split(self, split):
        self.c_params.split = split

    @property
    def use_batch(self):
        return self.c_params.use_batch
    @use_batch.setter
    def use_batch(self, use_batch):
        self.c_params.use_batch = use_batch
    
    @property
    def scorer(self):
        return self.c_params.scorer
    @scorer.setter
    def scorer(self, scorer):
        self.c_params.set_scorer(scorer.encode())
    
    @property
    def pop_size(self):
        return self.c_params.pop_size
    @pop_size.setter
    def pop_size(self, pop_size):
        self.c_params.pop_size = pop_size
