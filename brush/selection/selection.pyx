# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = selection.cc

from Selection cimport Selection

cdef class PySelection:
    cdef Selection sel

    def __cinit__(self, string type, bool survivor):
        self.sel = Selection(string type, bool survivor)

    def get_type(self):
        return self.sel.get_type()

    #TODO change matrixXf in select and survive to float and flatten them as fortran arrays

    cdef select(self, Population &pop,
                const MatrixXf &F,
                const Parameters &params):
        return self.sel.select(pop, F, params)

    cdef survive(self, Population &pop,
                 const MatrixXf &F,
                 const Parameters &params):
        return self.sel.survive(pop, F, params)
