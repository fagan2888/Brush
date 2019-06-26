# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: sources = variation.cc

from libcpp.vector cimport vector

from brush.variation.variation cimport CVariation
from brush.population.population cimport Population
from brush.params.params cimport Parameters
from brush.data.data cimport Data


cdef class Variation:
    # cdef CVariation c_vary

    def _cinit_(self, float cr):
        self.c_vary = CVariation(cr)

    def set_cross_rate(self, float cr):
        self.c_vary.set_cross_rate(cr)

    def get_cross_rate(self):
        return self.c_vary.get_cross_rate()

    def vary(self, Population pop,
              const vector[size_t]& parents,
              Parameters params,
              Data d):
        self.c_vary.vary(pop.c_pop, parents, params.c_params, d.cdata)
