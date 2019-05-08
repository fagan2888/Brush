# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = selection.cc

from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from eigency.core cimport *

from brush.selection.selection cimport CSelection
from brush.population.population cimport Population
from brush.params.params cimport Parameters

cdef class Selection:
    cdef CSelection sel

    def __cinit__(self, string type='lexicase', bool survival=False):
        self.sel = CSelection(type, survival)

    def get_type(self):
        return self.sel.get_type()

    #TODO change matrixXf in select and survive to float and flatten them as fortran arrays

    cdef select(self, Population &pop,
                const Parameters &params):
        return self.sel.select(pop.pop, params.params)

    cdef survive(self, Population &pop,
                 const Parameters &params):
        return self.sel.survive(pop.pop, params.params)
