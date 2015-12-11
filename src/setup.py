from setuptools import setup, find_packages
from distutils.core import setup, Extension
import os

curr_dir = os.path.dirname(os.path.abspath(__file__))
dll_path = [curr_dir]
dll_path = [os.path.join(p, 'libdtw_c.so') for p in dll_path]

lib_path = [p for p in dll_path if os.path.exists(p) and os.path.isfile(p)]

if len(lib_path) == 0:
    raise ("dtw_c library not found. Did you run ../make?")

setup(name='tsdistance',
      version='0.1',
      description='Dynamic Time Warping',
      py_modules=['tsdistance'],
      data_files=[('.', [lib_path[0]])],
)

