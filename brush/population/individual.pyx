# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = individual.cc

from libcpp cimport bool
from libcpp.vector cimport vector

from brush.population.individual cimport CIndividual
from brush.data.data import Data
from brush.params.params import Parameters

cdef class Individual:
    cdef CIndividual ind

    def __cinit__(self):
        self.ind = CIndividual()

    cdef out(self, const Data &d, const Parameters &params, bool predict):
        return self.ind.out(d.cdata, params.params, predict)

    cdef fit(self, const Data &d,
             const Parameters &params, bool &pass_val):
        return self.ind.fit(d.cdata, params.params, pass_val)

    cdef predict(self, const Data &d,
                 const Parameters &params):
        return self.ind.predict(d.cdata, params.params)

    def get_eqn(self):
        return self.ind.get_eqn()

    def get_features(self):
        return self.ind.get_features()

    def program_str(self):
        return self.ind.program_str()

    def set_rank(self, unsigned r):
        self.ind.set_rank(r)

    def get_n_params(self):
        return self.ind.get_n_params()

    def get_dim(self):
        return self.ind.get_dim()

    def complexity(self):
        return self.ind.complexity()

    def get_complexity(self):
        return self.ind.get_complexity()

    def set_id(self, unsigned i):
        self.ind.set_id(i)

    cdef set_parents(self, const vector[Individual] &parents):
        self.ind.set_parents(parents)

    cdef set_parents(self, const vector[int] &parents):
        self.ind.set_parents(parents)

    def get_p(self):
        return self.ind.get_p()

    def get_p(self, const size_t i, bool normalize):
        return self.ind.get_p(i, normalize)

    cdef get_p(self, const vector[size_t] &locs, bool normalize):
        return self.ind.get_p(locs, normalize)

    cdef set_p(self, const vector[float] &weights, const float &fb,
               const bool softmax_norm):
        self.ind.set_p(weights, fb, softmax_norm)

