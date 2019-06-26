# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = individual.cc

from libcpp cimport bool
from libcpp.vector cimport vector
from eigency.core cimport *
from brush.population.individual cimport CIndividual
from brush.data.data cimport Data
from brush.params.params cimport Parameters
from sklearn.utils import check_X_y

cdef class Individual:
    # cdef CIndividual ind

    def __cinit__(self, new=True):
        if new:
            self.ind = new CIndividual()
        
    cdef wrap(self, CIndividual* newind):
        if newind == NULL:
            raise Exception('newind pointer is null')
        self.ind = newind
        return self

    def __dealloc__(self):
        del self.ind

    def out(self, Data d, Parameters params, bool predict):
        res = ndarray(self.ind.out(d.cdata, params.c_params, predict))
        return res.flatten()

    def fit(self, Data d,
             Parameters params, bool &pass_val):
             
        res = ndarray(self.ind.fit(d.cdata, params.c_params, pass_val))
        return res.flatten()

    def predict(self, Data d,
                 Parameters params):
        res = ndarray(self.ind.predict(d.cdata, params.c_params))
        return res.flatten()

    def get_eqn(self):
        return self.ind.get_eqn()

    def get_features(self):
        return self.ind.get_features()

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

    def get_p(self):
        return self.ind.get_p()

    def set_p(self, const vector[float] &weights, const float &fb,
               const bool softmax_norm):
        self.ind.set_p(weights, fb, softmax_norm)

    @property
    def fitness(self):
        return self.ind.fitness
    @fitness.setter
    def fitness(self, fitness):
        self.ind.fitness = fitness
    
    @property
    def fitness_v(self):
        return self.ind.fitness_v
    @fitness_v.setter
    def fitness_v(self, fitness_v):
        self.ind.fitness_v = fitness_v
    
    @property
    def yhat(self):
        return ndarray(self.ind.yhat)
    @yhat.setter
    def yhat(self, np.ndarray yhat):
        check_X_y(yhat,ensure_min_samples=1)
        cdef np.ndarray[np.float32_t, ndim=1, mode="fortran"] arr
        arr = np.asfortranarray(yhat, dtype=np.float32)
        self.ind.set_yhat(Map[VectorXf](arr))
    
    @property
    def error(self):
        return ndarray(self.ind.error)
    @error.setter
    def error(self, np.ndarray error):
        check_X_y(error,ensure_min_samples=1)
        cdef np.ndarray[np.float32_t, ndim=1, mode="fortran"] arr
        arr = np.asfortranarray(error, dtype=np.float32)
        self.ind.set_error(Map[VectorXf](arr))

    @property
    def id(self):
        return self.ind.id
    @id.setter
    def id(self, id):
        self.ind.id = id
