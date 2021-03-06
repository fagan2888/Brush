# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.memory cimport unique_ptr
from libcpp.map cimport map
from libcpp.utility cimport pair
from eigency.core cimport *

cdef extern from "src/data.h" namespace "FT::Dat":
    cdef cppclass CData:    
        CData() except +
            
        CData(float *, int, int,
              float *, int,
              bool) except +
              
        CData(float *, int, int,
              float *, int,
              string, int *, int,
              bool) except +

        void set_validation(bool)
                
        void get_batch(CData &, int) const
        
        MatrixXf &get_X()
        void set_X(Map[MatrixXf] &)

        VectorXf &get_y()
        void set_y(Map[VectorXf] &)

    # cdef cppclass CCVData:
    #     CCVData() except +
        
    #     CCVData(float *, int, int,
    #           float *, int,
    #           bool) except +
              
    #     CCVData(float *, int, int,
    #           float *, int,
    #           string, int *, int,
    #           bool) except +
        
    #     void shuffle_data()
        
    #     void split_stratified(float)
        
    #     void train_test_split(bool, float)
        
    #     void get_batch(CCVData*, int)
        
    #     # void setValidationData(CData* d)
    #     # void setTrainingData(CData* d)

    #     CData* o
    #     CData* t
    #     CData* v
        
# forward declarations
cdef class Data:
    cdef CData cdata
    
# cdef class CVData:
#     cdef CCVData* c_cvdata
    # cdef get_batch(self, int batch_size)
