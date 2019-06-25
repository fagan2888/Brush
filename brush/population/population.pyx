# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = population.cc

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.string cimport string

from brush.population.population cimport CPopulation
from brush.population.individual cimport Individual
from brush.params.params cimport Parameters

cdef class Population:
    # cdef CPopulation pop

    def __cinit__(self):
        self.pop = CPopulation()

    def __cinit__(self, int p):
        self.pop = CPopulation(p)

    # def init(self, Individual starting_model,
    #           Parameters params,
    #           bool random):
    #     self.pop.init(starting_model.ind, params.c_params, random)
    def init(self, Parameters params, bool random):
        self.pop.init(params.c_params, random)

    def resize(self, int pop_size,
               bool resize_locs):
        self.pop.resize(pop_size, resize_locs)

    def update(self, vector[size_t] survivors):
        self.pop.update(survivors)

    def size(self):
        return self.pop.size()

    def add(self, Individual ind):
        self.pop.add(ind.ind)

    #TODO check how to do operator overloading
    #cdef operator[](self, size_t i):
    #    return self.pop[i]

    def print_eqns(self, bool just_offspring, string sep):
        return self.pop.print_eqns(just_offspring, sep)

    def sorted_front(self, unsigned rank):
        return self.pop.sorted_front(rank)
        
