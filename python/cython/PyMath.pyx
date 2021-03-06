import numpy
#cimport numpy as np
cimport cython

from libcpp.vector cimport vector as std_vector

from Math cimport *

cdef class PyMath:
    cdef Math *thisptr

    def __cinit__(self):
        self.thisptr = new Math()

    def __dealloc__(self):
        del self.thisptr

    def E1(self, double x):
        return self.thisptr.E1(x)

    def En(self, int n, double x):
        return self.thisptr.En(n, x)

    def deBoerD(self, double x):
        return self.thisptr.deBoerD(x)

    def deBoerL0(self, double mu1, double mu2, double muj, double density = 0.0, double thickness = 0.0):
        """
        The case the product density * thickness is 0.0 is for calculating the thick target limit
        """
        return self.thisptr.deBoerL0(mu1, mu2, muj, density, thickness)

    def erf(self, double x):
        """
        Calculate the error function erf(x)
        """
        return self.thisptr.erf(x)

    def erfc(self, double x):
        """
        Calculate the complementary error function erfc(x)
        """
        return self.thisptr.erfc(x)

    def hypermet(self, x, gaussArea, position, fwhm,
                 shortTailArea=0.0, shortTailSlope=0.03,
                 longTailArea=0.0, longTailSlope=1.0,
                 stepHeight=0.0):
        """
        Calculate the HYPERMET function of x.
        """
        return self.thisptr.hypermet(x, gaussArea, position, fwhm,
                                     shortTailArea, shortTailSlope, longTailArea, longTailSlope, stepHeight)
