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
from brush.population.individual cimport Individual
from brush.population.population cimport Population
from brush.params.params cimport Parameters
from brush.data.data cimport Data

cdef class Evaluation:
    cdef CEvaluation evalobj

    def __cinit__(self, string scorer):
        self.evalobj = CEvaluation(scorer)
        
    def set_score(self, string scorer):
        self.evalobj.set_score(scorer)

    cdef fitness(self, Population& pop,
                 const Data &d, 
                 const Parameters &params, 
                 bool offspring,
                 bool validation):
        self.evalobj.fitness(pop.pop,d.cdata,params.params,offspring,validation)  

    #TODO CLabels being used here need to remove
    cdef assign_fit(self, Individual &ind, const VectorXf &yhat, 
                    const VectorXf &y, const Parameters &params, bool val):
        self.evalobj.assign_fit(ind.ind,yhat,y,params.params,val)
