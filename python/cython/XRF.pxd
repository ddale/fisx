#import numpy as np
#cimport numpy as np
cimport cython

from libcpp.string cimport string as std_string
from libcpp.vector cimport vector as std_vector
from libcpp.map cimport map as std_map

from Elements cimport *

cdef extern from "fisx_xrf.h":
    cdef cppclass XRF:
        XRF() except +
        XRF(std_string) except +
        void readConfigurationFromFile(std_string) except +
        void setBeam(std_vector[double], std_vector[double], std_vector[int], std_vector[double]) except +

        void setGeometry(double, double, double) except +

        std_map[std_string, std_map[std_string, double]] getFluorescence(std_string, \
                Elements, int, std_string, int) except +


