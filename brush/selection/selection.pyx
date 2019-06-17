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
    # cdef CSelection sel

    def __cinit__(self, string name, bool survival=False):
        self.sel = CSelection(name, survival)

    def get_type(self):
        return self.sel.get_type()

    #TODO change matrixXf in select and survive to float and flatten them as 
    #fortran arrays

    def select(self, Population pop,
                Parameters params):
        return self.sel.select(pop.pop, params.c_params)

    def survive(self, Population pop,
                 Parameters params):
        return self.sel.survive(pop.pop, params.c_params)
