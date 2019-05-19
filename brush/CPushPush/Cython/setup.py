#from distutils.core import setup
#from Cython.Build import cythonize
import numpy
import sys
from glob import glob
from setuptools import setup, find_packages
from setuptools.extension import Extension
from Cython.Build import cythonize
import subprocess

setup(
    name="CPushPush",
    author="",
    author_email="",
    url="",
    download_url="",
    description="",
    zip_safe=True,
    install_requires=['Numpy>=1.8.2', 'SciPy>=0.13.3', 'Cython', 'pandas'],
    
    ext_modules = cythonize([
        # Extension(name='pushRunner',
        # sources=['pushRunner.pyx'],
        # include_dirs=['../src/', numpy.get_include()],
        # extra_compile_args=['-std=c++1y', '-fopenmp',],
        # language='c++'),

        Extension(name='individualRunner',
        sources=['individual.pyx'] + glob('../src/*.cpp'),
        include_dirs=['../src/', numpy.get_include()],
        extra_compile_args=['-std=c++1y', '-fopenmp',],)
        ],)
        # include_dirs=[numpy.get_include()])
)