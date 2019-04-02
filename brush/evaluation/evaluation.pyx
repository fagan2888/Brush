# distutils: language = c++
# distutils: sources = evaluation.cc
# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""


from brush.evaluation.evaluation cimport CEvaluation
from ..pop import Individual
from ..data import Data

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
                 bool validation)
          
    #TODO CLabels being used here need to remove
    cdef assign_fit(self, Individual &ind,
                    MatrixXf &F, const shared_ptr[CLabels] &yhat, 
                    const VectorXf &y, const Parameters &params, bool val)      
