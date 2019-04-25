#from distutils.core import setup
import sys
from setuptools import setup, find_packages
from setuptools.extension import Extension
from Cython.Build import cythonize
import subprocess
# the setup file relies on eigency to import its include paths for the
# extension modules. however eigency isn't known as a dependency until after
# setup is parsed; so we need to check for and install eigency before setup.
import importlib
try:
    importlib.import_module('eigency')
except ImportError:
    try:
        print('trying to install eigency prior to setup..')
        subprocess.check_call([sys.executable, '-m', 'pip', 'install', 'eigency'])
    except Exception as e:
        print(e)
        raise ImportError('The eigency library must be installed before Brush. '
                          'Automatic install with pip failed.')
finally:
    globals()['eigency'] = importlib.import_module('eigency')

package_version = '0.0'
import os
env_params = os.environ.keys() 

# get path to Brush shared library for linking
# cwd = '/'.join(os.getcwd().split('/')[:-1])
brush_lib = os.getcwd() + '/build/'

exts = []
for n in ['data','evaluation','params','population','program','selection','util',
         'variation']:
# for n in ['individual']:
    nfold = 'brush/'+n+'/'
    if n=='util':
        n = 'rnd'
    elif n =='individual':
        nfold='brush/population/'
    elif n == 'node':
        nfold='brush/program/'
    exts.append(
        Extension(name=n,
                  sources =  [nfold + n +'.pyx'],    # our cython source
                  include_dirs = ['.','build/',nfold,nfold+'src/']
                                  +eigency.get_includes(include_eigen=True),
                  extra_compile_args = ['-std=c++1y','-fopenmp','-Wno-sign-compare',
                                         '-Wno-reorder'],
                  library_dirs = [brush_lib], 
                  runtime_library_dirs = [brush_lib], 
                  extra_link_args = ['-lbrush'],      
                  language='c++'
                  ),
            )
print('exts:',exts)
setup(
    name="Brush",
    author='William La Cava',
    author_email='williamlacava@gmail.com',
    url = 'https://epistasis.github.io/brush',
    download_url='https://github.com/epistasis/brush/releases/tag/'+package_version,
    license='GNU/GPLv3',
    description='A package for fast genetic programming',
    zip_safe=True,
    install_requires=['Numpy>=1.8.2','SciPy>=0.13.3','scikit-learn','Cython','pandas'],
    ext_modules = cythonize(exts, language="c++"),
    include_package_data=True
    )
