# -*- coding: utf-8 -*-
"""
Copyright 2019 William La Cava
license: GNU/GPLv3
"""

# distutils: language=c++
# distutils: sources = node.cc

from Node cimport Node

cdef class PyNode:
    cdef Node nodeobj
    
    def __cinit__(self):
        self.node = Node()
    
    def clone(self) const:
        return self.node.clone()
        
    def rnd_clone(self) const:
        return self.node.clone()
        
