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
from sklearn.utils import check_X_y

cdef class Data:

    cdef CData cdata
    
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
                               
    # def init_with_z(self,np.ndarray X,np.ndarray y, string zfile, np.ndarray zids, 
    #         bool classification):
    #     cdef np.ndarray[np.float32_t, ndim=2, mode="fortran"] arr_x
    #     cdef np.ndarray[np.float32_t, ndim=1, mode="fortran"] arr_y
    #     cdef np.ndarray[int, ndim=1, mode="fortran"] arr_z_id
    #     check_X_y(X,y,ensure_2d=True,ensure_min_samples=1)
    #     X = X.transpose()
    #     arr_x = np.asfortranarray(X, dtype=np.float32)
    #     arr_y = np.asfortranarray(y, dtype=np.float32)

    def set_validation(self, bool v):
        self.cdata.set_validation(v)
        
    def get_batch(self, Data db, int batch_size):
        self.cdata.get_batch(db.cdata, batch_size)
        
cdef class CVData:
    cdef CCVData cvdata
    
    def init(self,np.ndarray X,np.ndarray y, bool classification):
        cdef np.ndarray[np.float32_t, ndim=2, mode="fortran"] arr_x
        cdef np.ndarray[np.float32_t, ndim=1, mode="fortran"] arr_y
        cdef np.ndarray[int, ndim=1, mode="fortran"] arr_z_id
        check_X_y(X,y,ensure_2d=True,ensure_min_samples=1)
        X = X.transpose()
        arr_x = np.asfortranarray(X, dtype=np.float32)
        arr_y = np.asfortranarray(y, dtype=np.float32)
        
        self.cvdata = CCVData(&arr_x[0,0],X.shape[0],X.shape[1],
                &arr_y[0],len(arr_y), classification)
    
    def init_with_z(self,np.ndarray X,np.ndarray y, string zfile, np.ndarray zids, 
            bool classification):
        cdef np.ndarray[np.float32_t, ndim=2, mode="fortran"] arr_x
        cdef np.ndarray[np.float32_t, ndim=1, mode="fortran"] arr_y
        cdef np.ndarray[int, ndim=1, mode="fortran"] arr_z_id
        check_X_y(X,y,ensure_2d=True,ensure_min_samples=1)
        X = X.transpose()
        arr_x = np.asfortranarray(X, dtype=np.float32)
        arr_y = np.asfortranarray(y, dtype=np.float32)
        arr_z_id = np.asfortranarray(zids, dtype=ctypes.c_int)
        
        self.cvdata = CCVData(&arr_x[0,0],X.shape[0],X.shape[1],
        		      &arr_y[0],len(arr_y),
                              zfile, &arr_z_id[0], len(arr_z_id), 
                              classification)
    
    def shuffle_data(self):
        self.cvdata.shuffle_data()
    
    def split_stratified(self, float split):
        self.cvdata.split_stratified(split)
    
    def train_test_split(self, bool shuffle, float split):
        self.cvdata.train_test_split(shuffle, split)

    def set_training_data(self, Data d):
        self.cvdata.setTrainingData(d.cdata)

    def set_validation_data(self, Data d):
        self.cvdata.setValidationData(d.cdata)

    def get_batch(self, int batch_size):
        db = Data()
        dbcv = CVData()
        self.cvdata.t.get_batch(db, batch_size)
        dbcv.set_training_data(db)
        dbcv.set_validation_data(self.cvdata.v)

        return dbcv
