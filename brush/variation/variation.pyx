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
    cdef CVariation vary_obj

    def _cinit_(self, float cr):
        self.vary_obj = CVariation(cr)

    def set_cross_rate(self, float cr):
        self.vary_obj.set_cross_rate(cr)

    def get_cross_rate(self):
        return self.vary_obj.get_cross_rate()

    cdef vary(self, Population& pop,
              const vector[size_t]& parents,
              const Parameters& params,
              const Data& d):
        self.vary_obj.vary(pop.pop, parents, params.c_params, d.cdata)
