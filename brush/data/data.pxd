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

cdef cppclass Data

cdef cppclass CVData

cdef extern from "src/data.h" namespace "FT::Dat":
    cdef cppclass CData:    
        CData() except +
            
        CData(MatrixXf *, VectorXf *, 
              map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] *, bool) except +

        void set_validation(bool)
                
        void get_batch(CData &, int) const
        
    cdef cppclass CCVData:
        CCVData() except +
        
        CCVData(MatrixXf *, VectorXf *, 
                map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] *, 
                bool) except +
                        
        void setOriginalData(MatrixXf *, VectorXf *, 
                              map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] *, bool)
        
        void setOriginalData(CData *)
        
        void setTrainingData(MatrixXf *, VectorXf *, 
                           map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] *,
                           bool)
        
        void setTrainingData(CData *, bool)
        
        void setValidationData(MatrixXf *, VectorXf *, 
                           map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] *,
                           bool)
        
        void setValidationData(CData *)
        
        void shuffle_data()
        
        void split_stratified(float)
        
        void train_test_split(bool, float)

        void split_longitudinal(map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] *,
                                map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] *,
                                map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] *,
                                float)
                    
        void reorder_longitudinal(vector[ArrayXf] &, const vector[int] &)
