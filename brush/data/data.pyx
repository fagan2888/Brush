# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

from brush.data.data cimport CData
import numpy as np 
import ctypes
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.memory cimport unique_ptr
from libcpp.map cimport map
from libcpp.utility cimport pair
from eigency.core cimport *
from sklearn.utils import check_X_y
from sklearn.model_selection import train_test_split

cdef class Data:

    # cdef CData cdata
    
    def __cinit__(self,np.ndarray X,np.ndarray y, bool classification,
                  string zfile='', np.ndarray zids=None):
        cdef np.ndarray[np.float32_t, ndim=2, mode="fortran"] arr_x
        cdef np.ndarray[np.float32_t, ndim=1, mode="fortran"] arr_y
        cdef np.ndarray[int, ndim=1, mode="fortran"] arr_z_id
        check_X_y(X,y,ensure_2d=True,ensure_min_samples=1)
        X = X.transpose()
        arr_x = np.asfortranarray(X, dtype=np.float32)
        arr_y = np.asfortranarray(y, dtype=np.float32)
        
        if zfile != '':
            arr_z_id = np.asfortranarray(zids, dtype=ctypes.c_int)
            
            self.cdata = CData(&arr_x[0,0],X.shape[0],X.shape[1],
                               &arr_y[0],len(arr_y),
                               zfile, &arr_z_id[0], len(arr_z_id),
                               classification)
        else:
            self.cdata = CData(&arr_x[0,0],X.shape[0],X.shape[1],
                               &arr_y[0],len(arr_y),
                               classification)
                               
    def set_validation(self, bool v):
        self.cdata.set_validation(v)
        
    def get_batch(self, int batch_size):
        db = Data()
        self.cdata.get_batch(db.cdata, batch_size)
        return db
        

    @property
    def X(self):
        return ndarray(self.cdata.get_X()) 
    @X.setter
    def X(self, np.ndarray X):
        check_X_y(X,ensure_2d=True,ensure_min_samples=1)
        X = X.transpose()
        cdef np.ndarray[np.float32_t, ndim=2, mode="fortran"] arr_x
        arr_x = np.asfortranarray(X, dtype=np.float32)
        self.cdata.set_X(Map[MatrixXf](arr_x)) 

    @property
    def y(self):
        return ndarray(self.cdata.get_y()) 
    @y.setter
    def y(self, np.ndarray y):
        check_X_y(y,ensure_2d=True,ensure_min_samples=1)
        cdef np.ndarray[np.float32_t, ndim=1, mode="fortran"] arr_y
        arr_y = np.asfortranarray(y, dtype=np.float32)
        self.cdata.set_y(Map[VectorXf](arr_y)) 

