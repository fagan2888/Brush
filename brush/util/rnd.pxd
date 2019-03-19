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

cdef extern from "rnd.h" namespace "FT::Util":
    cdef cppclass Rnd:
        
        @staticmethod
        Rnd* initRand()
        
        @staticmethod
        void destroy()

        void set_seed(int)
        
        int rnd_int( int, int)

        float rnd_flt(float min, float max)

        float rnd_dbl(float min, float max)
        
        float operator()(unsigned)
        
        float operator()()
        
	    void shuffle[RandomAccessIterator] (RandomAccessIterator, RandomAccessIterator)    
        
        T select_randomly[T](T, T)
               
        T random_choice[T](const vector[T] &)
       
        T random_choice[T][D](const vector[T] &, const vector[D] &)
        
        float gasdev()
