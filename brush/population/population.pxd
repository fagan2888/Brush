# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# TODO check how inheritance works in cython
# need to inherit selectionoperator class here

import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "population.h" namespace "FT::Pop":
    cdef cppclass CPopulation:

        CPopulation() except +

        CPopulation(int) except +

        void init(const CIndividual &,
                  const CParameters &,
                  bool random)

        void resize(int, bool)

        void update(vector[size_t])

        int size()

        size_t get_open_loc()

        void update_open_loc()

        void add(Individual &)

        const Individual operator [](size_t) const

        const Individual & operator [](size_t)

        string print_eqns(bool, string)

        vector[size_t] sorted_front(unsigned)
