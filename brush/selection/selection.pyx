# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = selection.cc

from brush.selection.selection cimport CSelection

cdef class Selection:
    cdef CSelection sel

    def __cinit__(self, string type, bool survivor):
        self.sel = CSelection(string type, bool survivor)

    def get_type(self):
        return self.sel.get_type()

    #TODO change matrixXf in select and survive to float and flatten them as fortran arrays

    cdef select(self, Population &pop,
                const MatrixXf &F,
                const Parameters &params):
        return self.sel.select(pop.pop, F, params.params)

    cdef survive(self, Population &pop,
                 const MatrixXf &F,
                 const Parameters &params):
        return self.sel.survive(pop.pop, F, params.params)
