# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = evaluation.cc

from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool
from eigency.core cimport *


from brush.evaluation.evaluation cimport CEvaluation
from brush.population.individual cimport Individual
from brush.population.population cimport Population
from brush.params.params cimport Parameters
from brush.data.data cimport Data

cdef class Evaluation:
    # cdef CEvaluation c_eval

    def __cinit__(self, string scorer):
        self.c_eval = CEvaluation(scorer)
        
    def set_score(self, string scorer):
        self.c_eval.set_score(scorer)

    def fitness(self, Population pop,
                 Data d, 
                 Parameters params, 
                 bool offspring,
                 bool validation=False):
        self.c_eval.fitness(pop.c_pop, d.cdata, params.c_params, offspring, 
                validation)  

    # def assign_fit(self, Individual ind, const VectorXf &yhat, 
    #                 const VectorXf &y, Parameters params, bool val):
    #     self.c_eval.assign_fit(ind.ind, yhat, y, params.c_params, val)

