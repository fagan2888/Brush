# distutils: language = c++
# distutils: sources = evaluation.cc
# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool
from eigency.core cimport *


from brush.evaluation.evaluation cimport CEvaluation
from brush.population.individual import Individual
from brush.params.params import Parameters
from brush.data import Data

cdef class Evaluation:
    cdef CEvaluation evalobj

    def __cinit__(self, string scorer):
        self.evalobj = Evaluation(scorer)
        
    def set_score(self, string scorer):
        self.evalobj.set_score(scorer)

    cdef fitness(self, vector[Individual] &ind,
                 const Data &d, 
                 MatrixXf &F, 
                 const Parameters &params, 
                 bool offspring,
                 bool validation):
        self.evalobj.fitness(ind.ind,d.cdata,F,params.params,offspring,validation)  

    #TODO CLabels being used here need to remove
    cdef assign_fit(self, Individual &ind,
                    MatrixXf &F, const VectorXf &yhat, 
                    const VectorXf &y, const Parameters &params, bool val):
        self.evalobj.assign_fit(ind.ind,F,yhat,y,params,val)
