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
# from collections.abc import Sequence

cdef class Population:
    """Defines a population of individuals. 

    Parameters:
    -----------

    p: int 
        population size. 
    """

    def __cinit__(self):
        self.c_pop = CPopulation()

    def __cinit__(self, int p):
        self.c_pop = CPopulation(p)

    # def init(self, Individual starting_model,
    #           Parameters params,
    #           bool random):
    #     self.c_pop.init(starting_model.ind, params.c_params, random)
    def init(self, Parameters params, bool random):
        self.c_pop.init(params.c_params, random)

    def resize(self, int pop_size,
               bool resize_locs):
        self.c_pop.resize(pop_size, resize_locs)

    def update(self, vector[size_t] survivors):
        self.c_pop.update(survivors)

    def size(self):
        return self.c_pop.size()

    def add(self, Individual ind):
        self.c_pop.add(ind.ind[0])

    #TODO check how to do operator overloading
    #cdef operator[](self, size_t i):
    #    return self.c_pop[i]

    def print_eqns(self, bool just_offspring, string sep):
        return self.c_pop.print_eqns(just_offspring, sep)

    def sorted_front(self, unsigned rank):
        return self.c_pop.sorted_front(rank)
        
    @property
    def individuals(self):
        individuals = []
        for ind in self.c_pop.individuals:
            individuals.append(Individual(new=False).wrap(&ind))
        return individuals

    # # define iterators to access individuals
    def __iter__(self):
        return iter(self.individuals)

    def __next__(self):
        return self.individuals.next()
