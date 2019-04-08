# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

from brush.data.data cimport CData
from brush.data.data cimport CCVData

import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.memory cimport unique_ptr
from libcpp.map cimport map
from libcpp.utility cimport pair
from eigency.core cimport *


cdef class Data:
    cdef CData cdata
    
    #TODO consult Bill regarding this
    cdef init(self, MatrixXf &X, VectorXf &y,
                   map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z,
                   bool c):
        self.cdata = CData(X, y, Z, c)

    def set_validation(self, bool v):
        self.cdata.set_validation(v)
        
    def get_batch(self, Data db, int batch_size):
        self.cdata.get_batch(db.cdata, batch_size)
        
cdef class CVData:
    cdef CCVData cvdata
    
    cdef init(self, MatrixXf & X, VectorXf& y, 
                   map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z, 
                   bool c):
        self.cvdata = CCVData(X, y, Z, c)
                        
    cdef setOriginalData(self, MatrixXf& X, VectorXf& y, 
                          map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z,
                          bool c):
        self.cvdata.setOriginalData(X, y, Z, c)
    
    def setOriginalDataFromObj(self, Data d):
        self.cvdata.setOriginalData(&(d.cdata))
    
    cdef setTrainingData(self, MatrixXf& X_t, VectorXf& y_t, 
                         map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z_t,
                         bool c):
        self.cvdata.setTrainingData(X_t, y_t, Z_t, c)
    
    def setTrainingDataFromObj(self, Data d, bool toDelete):
        self.cvdata.setTrainingData(&(d.cdata), toDelete)
    
    cdef setValidationData(self, MatrixXf& X_v, VectorXf& y_v, 
                           map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z_v,
                           bool c):
        self.cvdata.setValidationData(X_v, y_v, Z_v, c)
    
    def setValidationDataFromObj(self, Data d):
        self.cvdata.setValidationData(&(d.cdata))
    
    def shuffle_data(self):
        self.cvdata.shuffle_data()
    
    def split_stratified(self, float split):
        self.cvdata.split_stratified(split)
    
    def train_test_split(self, bool shuffle, float split):
        self.cvdata.train_test_split(shuffle, split)

    cdef split_longitudinal(self, 
                            map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z,
                            map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z_t,
                            map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z_v,
                            float split):
        self.cvdata.split_longitudinal(Z, Z_t, Z_v, split)
                
    cdef reorder_longitudinal(self, vector[ArrayXf] &vec1, const vector[int] &order):
        self.cvdata.reorder_longitudinal(vec1, order)
