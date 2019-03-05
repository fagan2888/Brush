# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = Population.cpp

from Population cimport Population

cdef class PyPopulation:
    cdef Population pop

    def __cinit__(self):
        self.pop = Population()

    def __cinit__(self, int p):
        self.pop = Population(p)

    cdef init(self, const Individual &starting_model,
              const Parameters &params,
              bool random):
        self.pop.init(starting_model, params, random)

    def resize(self, int pop_size,
               bool resize_locs):
        self.pop.resize(pop_size, resize_locs)

    def update(self, vector[size_t] survivors):
        self.pop.update(survivors)

    def size(self):
        return self.pop.size()

    def get_open_loc(self):
        return self.pop.get_open_loc()

    def update_open_loc(self):
        self.pop.update_open_loc()

    cdef add(self, Individual &ind):
        self.pop.add(ind)

    def operator [](self, size_t i) const:
        return self.pop[i]

    def operator [](size_t i):
        return self.pop[i]

    def print_eqns(self, bool just_offspring, string sep):
        return self.pop.print_eqns(just_offspring, sep)

    def sorted_front(self, unsigned rank):
        return self.pop.sorted_front(rank)

#TODO check with bill if sortcomplexity and samefitcomplexity need to be exposed or not
