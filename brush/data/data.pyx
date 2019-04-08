# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

from brush.data.data cimport CData
from brush.data.data cimport CCVData

cdef class Data:
    cdef CData cdata
    
    cdef __cinit__(self, MatrixXf &X, VectorXf &y,
                   std::map<string, std::pair<vector<ArrayXf>, vector<ArrayXf>>> &Z,
                   bool c):
        self.cdata = CData(X, y, Z, c)

    def set_validation(bool v):
        self.cdata.set_validation(v)
        
    def get_batch(self, CData &db, int batch_size) const:
        self.cdata.get_batch(db, batch_size)
        
cdef class CVData:
    cdef CCVData cvdata
    
    cdef __cinit__(self, MatrixXf & X, VectorXf& y, 
                   map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z, 
                   bool c):
        self.cvdata = CCVData(X, y, Z, c)
                        
    cdef setOriginalData(self, MatrixXf& X, VectorXf& y, 
                          map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z,
                          bool c):
        self.cvdata.setOriginalData(X, y, Z, c)
    
    cdef setOriginalData(self, Data d):
        self.cvdata.setOriginalData(d.cdata)
    
    cdef setTrainingData(self, MatrixXf& X_t, VectorXf& y_t, 
                         map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z_t,
                         bool c):
        self.cvdata.setTrainingData(X_t, y_t, Z_t, c)
    
    def setTrainingData(self, Data d, bool toDelete):
        self.cvdata.setTrainingData(d.cdata, toDelete)
    
    cdef setValidationData(self, MatrixXf& X_v, VectorXf& y_v, 
                            map[string, pair[vector[ArrayXf], vector[ArrayXf] ] ] &Z_v,
                            bool c):
        self.cvdata.setValidationData(X_v, y_v, Z_v, c)
    
    def setValidationData(self, Data d):
        self.cvdata.setValidationData(d.cdata)
    
    def shuffle_data(self):
        self.cvdata.shuffle_data()
    
    def split_stratified(self, float split)
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
