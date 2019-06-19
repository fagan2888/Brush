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

# cdef class CVData:
#     """A mostly Python class that manages training and validation data"""
#     # def __init__(self, np.ndarray X, np.ndarray y, bool classification,
#     #         np.ndarray X_val=None, np.ndarray y_val= None):
#     cdef Data train
#     cdef Data val
#     cdef bool classification, shuffle
#     cdef float split

#     def __cinit__(self, np.ndarray X, np.ndarray y, bool classification,
#                  bool shuffle, float split, random_state=None):
#         self.classification = classification
#         self.shuffle = shuffle
#         self.split = split

#         if split < 1.0:
#             self._split(X,y)

#     def _split(self, X,y):
#         stratify = y if self.classification else None
#         X_train, X_val, y_train, y_val = train_test_split(X, y, 
#                 test_size=1-self.split, shuffle = self.shuffle,
#                 stratify = stratify)
#         self.train = Data(X_train, y_train, self.classification)
#         self.val = Data(X_val, y_val, self.classification)

#     def get_batch(self, int batch_size):
#         """returns a new CVData object with batch sampled training data 
#         and the same validation data as this CVData object"""
#         newdata = self
#         newdata.train = self.train.get_batch(batch_size)
#         return newdata
        
# cdef class CVData:
#     """Includes train and validation folds data objects"""
#     # cdef CCVData c_cvdata
     
#     def __cinit__(self, np.ndarray X, np.ndarray y, bool classification):
#         cdef np.ndarray[np.float32_t, ndim=2, mode="fortran"] arr_x
#         cdef np.ndarray[np.float32_t, ndim=1, mode="fortran"] arr_y
#         # cdef np.ndarray[int, ndim=1, mode="fortran"] arr_z_id
#         check_X_y(X,y,ensure_2d=True,ensure_min_samples=1)
#         X = X.transpose()
#         arr_x = np.asfortranarray(X, dtype=np.float32)
#         arr_y = np.asfortranarray(y, dtype=np.float32)
        
#         # print('arr_x:',arr_x)
#         # print('y:',arr_y)
#         print('data.pyx: calling CCVData')
#         self.c_cvdata = new CCVData(&arr_x[0,0],X.shape[0],X.shape[1],
#                               &arr_y[0], len(arr_y), classification)
#         print('self.c_cvdata set')
#         print('dir(self):',dir(self))

#     def __dealloc__(self):
#         del self.c_cvdata 
#     # def init_with_z(self,np.ndarray X,np.ndarray y, string zfile, 
#     #           np.ndarray zids, bool classification):
#     #     cdef np.ndarray[np.float32_t, ndim=2, mode="fortran"] arr_x
#     #     cdef np.ndarray[np.float32_t, ndim=1, mode="fortran"] arr_y
#     #     cdef np.ndarray[int, ndim=1, mode="fortran"] arr_z_id
#     #     check_X_y(X,y,ensure_2d=True,ensure_min_samples=1)
#     #     X = X.transpose()
#     #     arr_x = np.asfortranarray(X, dtype=np.float32)
#     #     arr_y = np.asfortranarray(y, dtype=np.float32)
#     #     arr_z_id = np.asfortranarray(zids, dtype=ctypes.c_int)
        
#     #     self.c_cvdata = CCVData(&arr_x[0,0],X.shape[0],X.shape[1],
#     #     		      &arr_y[0],len(arr_y),
#     #                           zfile, &arr_z_id[0], len(arr_z_id), 
#     #                           classification)
    
#     def shuffle_data(self):
#         self.c_cvdata.shuffle_data()
    
#     def split_stratified(self, float split):
#         self.c_cvdata.split_stratified(split)
    
#     def train_test_split(self, bool shuffle, float split):
#         self.c_cvdata.train_test_split(shuffle, split)

#     # def set_training_data(self, Data d):
#     #     self.c_cvdata.setTrainingData(d.cdata)

#     # def set_validation_data(self, Data d):
#     #     self.c_cvdata.setValidationData(d.cdata)

#     def get_batch(self, int batch_size):
#         dbcv = CVData()
#         self.c_cvdata.get_batch(dbcv.c_cvdata, batch_size)

#         return dbcv

#     # Attribute access for X,y train and test splits
#     @property
#     def X_train(self):
#         return self.c_cv_data.t.X
#     @property
#     def X_test(self):
#         return self.c_cv_data.v.X
#     @property
#     def y_train(self):
#         return self.c_cv_data.t.y
#     @property
#     def y_test(self):
#         return self.c_cv_data.v.y
