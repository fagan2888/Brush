# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = Individual.cpp

from Individual cimport Individual

cdef class PyIndividual:
    cdef Individual ind

    def __cinit__(self):
        self.ind = Individual()

    cdef out(self, const Data &d, const Parameters &params, bool predict):
        return self.ind.out(d, params, predict)

    cdef out_trace(self, const Data &d,
                   const Parameters &params,
                   vector[Trace] &stack_trace):
        self.ind.out_trace(d, params, stack_trace)

    cdef fit(self, const Data &d,
             const Parameters &params):
        return self.out(d, params, False)

    cdef predict(self, const Data &d,
                 const Parameters &params):
        return self.out(d, params, True)

    def get_eqn(self):
        return self.ind.get_eqn()

    def get_features(self):
        return self.ind.get_features()

    def program_str(self) const:
        return self.ind.program_str()

    def set_rank(self, unsigned r):
        self.ind.set_rank(r)

    def get_n_params(self):
        return self.ind.get_n_params()

    def get_dim(self):
        return self.ind.get_dim()

    def complexity(self):
        return self.ind.complexity()

    def get_complexity(self) const:
        return self.ind.get_complexity()

    def set_id(self, unsigned i):
        self.ind.set_id(i)

    cdef set_parents(self, const vector[Individual] &parents):
        self.ind.set_parents(parents)

    cdef set_parents(self, const vector[int] &parents):
        self.ind.set_parents(parents)

    def get_p(self) const:
        return self.ind.get_p()

    def get_p(self, const size_t i, bool normalize) const:
        return self.ind.get_pp(i, normalize)

    cdef get_p(self, const vector[size_t] &locs, bool normalize) const:
        return self.ind.get_p(locs, normalize)

    cdef set_p(self, const vector[float] &weights, const float &fb,
               const bool softmax_norm):
        self.ind.set_p(weights, fb, softmax_norm)

