import numpy
#cimport numpy as np
cimport cython

from cython.operator cimport dereference as deref
from libcpp.string cimport string as std_string
from libcpp.vector cimport vector as std_vector
from libcpp.map cimport map as std_map
from libcpp.map cimport pair as std_pair
from operator import itemgetter

from Elements cimport *
from Material cimport *
from Layer cimport *

cdef class PyLayer:
    cdef Layer *thisptr

    def __cinit__(self, materialName, double density=1.0, double thickness=1.0, double funny=1.0):
        self.thisptr = new Layer(toBytes(materialName), density, thickness, funny)

    def __dealloc__(self):
        del self.thisptr

    def getTransmission(self, energies, PyElements elementsLib, double angle=90.):
        if not hasattr(energies, "__len__"):
            energies = numpy.array([energies], numpy.float)
        return self.thisptr.getTransmission(energies, deref(elementsLib.thisptr), angle)

    def setMaterial(self, PyMaterial material):
        self.thisptr.setMaterial(deref(material.thisptr))

    def getPeakFamilies(self, double energy, PyElements elementsLib):
        tmpResult = self.thisptr.getPeakFamilies(energy, deref(elementsLib.thisptr))
        return sorted(tmpResult, key=itemgetter(1))

