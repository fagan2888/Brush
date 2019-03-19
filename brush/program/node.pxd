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
from eigency.core cimport *

cdef extern from "node.h" namespace "FT::Pop::Op":
    cdef struct Node:

        Node() except +
        
        unique_ptr[Node] clone() const
                    
        unique_ptr[Node] rnd_clone() const
