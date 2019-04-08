# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = node.cc

from brush.program.node cimport CNode

cdef class Node:
    cdef CNode nodeobj
    
    def __cinit__(self):
        self.nodeobj = CNode()
    
    def clone(self):
        return self.nodeobj.clone()
        
    def rnd_clone(self):
        return self.nodeobj.clone()
        
