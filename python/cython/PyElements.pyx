import sys
cimport cython

from operator import itemgetter
from libcpp.string cimport string as std_string
from libcpp.vector cimport vector as std_vector
from libcpp.map cimport map as std_map

from Elements cimport *
from Material cimport *

__doc__ = """

Initialization with XCOM parameters from fisx:

import os
from fisx import DataDir
dataDir = DataDir.DATA_DIR
bindingEnergies = os.path.join(dataDir, "BindingEnergies.dat")
xcomFile = os.path.join(dataDir, "XCOM_CrossSections.dat")
xcom = Elements(dataDir, bindingEnergies, xcomFile)

Initialization with XCOM parameters from PyMca:

import os
from PyMca5 import PyMcaDataDir
dataDir = PyMcaDataDir.PYMCA_DATA_DIR
bindingEnergies = os.path.join(dataDir, "BindingEnergies.dat")
xcomFile = os.path.join(dataDir, "XCOM_CrossSections.dat")
# This is needed because PyMca does not have the non-radiative rates
from fisx import DataDir
dataDir = DataDir.DATA_DIR
xcom = Elements(dataDir, bindingEnergies, xcomFile)

"""
cdef class PyElements:
    cdef Elements *thisptr

    def __cinit__(self, directoryName="",
                        bindingEnergiesFile="",
                        crossSectionsFile=""):
        if len(directoryName) == 0:
            from fisx import DataDir
            directoryName = DataDir.DATA_DIR
        directoryName = toBytes(directoryName)
        bindingEnergiesFile = toBytes(bindingEnergiesFile)
        crossSectionsFile = toBytes(crossSectionsFile)
        if len(bindingEnergiesFile):
            self.thisptr = new Elements(directoryName, bindingEnergiesFile)
        else:
            self.thisptr = new Elements(directoryName)
        if len(crossSectionsFile):
            self.thisptr.setMassAttenuationCoefficientsFile(crossSectionsFile)

    def initializeAsPyMca(self):
        import os
        try:
            from fisx import DataDir
            directoryName = DataDir.DATA_DIR
            from PyMca5 import PyMcaDataDir
            dataDir = PyMcaDataDir.PYMCA_DATA_DIR
        except ImportError:
            from fisx import DataDir
            directoryName = DataDir.DATA_DIR
            dataDir = directoryName
        bindingEnergies = os.path.join(dataDir, "BindingEnergies.dat")
        xcomFile = os.path.join(dataDir, "XCOM_CrossSections.dat")
        del self.thisptr
        self.thisptr = new Elements(directoryName, bindingEnergies, xcomFile)
        for shell in ["K", "L", "M"]:
            shellConstantsFile = os.path.join(dataDir, shell+"ShellConstants.dat")
            self.thisptr.setShellConstantsFile(toBytes(shell), toBytes(shellConstantsFile))

        for shell in ["K", "L", "M"]:
            radiativeRatesFile = os.path.join(dataDir, shell+"ShellRates.dat")
            self.thisptr.setShellRadiativeTransitionsFile(toBytes(shell), toBytes(radiativeRatesFile))

    def getElementNames(self):
        return self.thisptr.getElementNames()

    def getComposition(self, materialOrFormula):
        if sys.version < "3.0":
            return self.thisptr.getComposition(toBytes(materialOrFormula))
        else:
            return toStringKeys(self.thisptr.getComposition(toBytes(materialOrFormula)))

    def __dealloc__(self):
        del self.thisptr

    def addMaterial(self, PyMaterial material, int errorOnReplace=1):
        self.thisptr.addMaterial(deref(material.thisptr), errorOnReplace)

    def setShellConstantsFile(self, mainShellName, fileName):
        """
        Load main shell (K, L or M) constants from file (fluorescence and Coster-Kronig yields)
        """
        self.thisptr.setShellConstantsFile(toBytes(mainShellName), toBytes(fileName))

    def getShellConstantsFile(self, mainShellName):
        if sys.version < "3.0":
            return self.thisptr.getShellConstantsFile(mainShellName)
        else:
            return toString(self.thisptr.getShellConstantsFile(toBytes(mainShellName)))

    def setShellRadiativeTransitionsFile(self, mainShellName, fileName):
        """
        Load main shell (K, L or M) X-ray emission rates from file.
        The library normalizes internally.
        """
        self.thisptr.setShellRadiativeTransitionsFile(toBytes(mainShellName), toBytes(fileName))

    def getShellRadiativeTransitionsFile(self, mainShellName):
        if sys.version < "3.0":
            return self.thisptr.getShellRadiativeTransitionsFile(mainShellName)
        else:
            return toString(self.thisptr.getShellRadiativeTransitionsFile(toBytes(mainShellName)))

    def getShellNonradiativeTransitionsFile(self, mainShellName):
        if sys.version < "3.0":
            return self.thisptr.getShellNonradiativeTransitionsFile(mainShellName)
        else:
            return toString(self.thisptr.getShellNonradiativeTransitionsFile(toBytes(mainShellName)))

    def setMassAttenuationCoefficients(self,
                                       std_string element,
                                       std_vector[double] energies,
                                       std_vector[double] photo,
                                       std_vector[double] coherent,
                                       std_vector[double] compton,
                                       std_vector[double] pair):
        self.thisptr.setMassAttenuationCoefficients(element,
                                                    energies,
                                                    photo,
                                                    coherent,
                                                    compton,
                                                    pair)
    def setMassAttenuationCoefficientsFile(self, crossSectionsFile):
        self.thisptr.setMassAttenuationCoefficientsFile(toBytes(crossSectionsFile))
    
    def _getSingleMassAttenuationCoefficients(self, std_string element,
                                                     double energy):
        if sys.version < "3.0":
            return self.thisptr.getMassAttenuationCoefficients(element, energy)
        else:
            return toStringKeys(self.thisptr.getMassAttenuationCoefficients(element, energy))

    def _getElementDefaultMassAttenuationCoefficients(self, std_string element):
        if sys.version < "3.0":
            return self.thisptr.getMassAttenuationCoefficients(element)
        else:
            return toStringKeys(self.thisptr.getMassAttenuationCoefficients(element))

    def getElementMassAttenuationCoefficients(self, element, energy=None):
        if energy is None:
            return self._getElementDefaultMassAttenuationCoefficients(toBytes(element))
        elif hasattr(energy, "__len__"):
            return self._getMultipleMassAttenuationCoefficients(toBytes(element),
                                                                       energy)
        else:
            return self._getMultipleMassAttenuationCoefficients(toBytes(element),
                                                                       [energy])

    def _getMultipleMassAttenuationCoefficients(self, std_string element,
                                                       std_vector[double] energy):
        if sys.version < "3.0":
            return self.thisptr.getMassAttenuationCoefficients(element, energy)
        else:
            return toStringKeys(self.thisptr.getMassAttenuationCoefficients(element, energy))

    def getMassAttenuationCoefficients(self, name, energy=None):
        if hasattr(name, "keys"):
            return self._getMaterialMassAttenuationCoefficients(toBytes(name), energy)
        elif energy is None:
            return self._getElementDefaultMassAttenuationCoefficients(toBytes(name))
        elif hasattr(energy, "__len__"):
            return self._getMultipleMassAttenuationCoefficients(toBytes(name), energy)
        else:
            # do not use the "single" version to have always the same signature
            return self._getMultipleMassAttenuationCoefficients(toBytes(name), [energy])

    def getExcitationFactors(self, name, energy, weight=None):
        if hasattr(energy, "__len__"):
            if weight is None:
                weight = [1.0] * len(energy)
            return self._getExcitationFactors(toBytes(name), energy, weight)[0]
        else:
            energy = [energy]
            if weight is None:
                weight = [1.0]
            else:
                weight = [weight]
            return self._getExcitationFactors(toBytes(name), energy, weight)

    def _getMaterialMassAttenuationCoefficients(self, elementDict, energy):
        """
        elementDict is a dictionary of the form:
        elmentDict[key] = fraction where:
            key is the element name
            fraction is the mass fraction of the element.

        WARNING: The library renormalizes in order to make sure the sum of mass
                 fractions is 1.
        """

        if hasattr(energy, "__len__"):
            return self._getMassAttenuationCoefficients(elementDict, energy)
        else:
            return self._getMassAttenuationCoefficients(elementDict, [energy])

    def _getMassAttenuationCoefficients(self, std_map[std_string, double] elementDict,
                                              std_vector[double] energy):
        return self.thisptr.getMassAttenuationCoefficients(elementDict, energy)

    def _getExcitationFactors(self, std_string element,
                                   std_vector[double] energies,
                                   std_vector[double] weights):
        if sys.version < "3.0":
            return self.thisptr.getExcitationFactors(element, energies, weights)
        else:
            return [toStringKeysAndValues(x) for x in self.thisptr.getExcitationFactors(element, energies, weights)]

    def getPeakFamilies(self, nameOrVector, energy):
        if type(nameOrVector) in [type([]), type(())]:
            if sys.version < "3.0":
                return sorted(self._getPeakFamiliesFromVectorOfElements(nameOrVector, energy), key=itemgetter(1))
            else:
                nameOrVector = [toBytes(x) for x in nameOrVector]
                return [(toString(x[0]), x[1]) for x in \
                        sorted(self._getPeakFamiliesFromVectorOfElements(nameOrVector, energy), key=itemgetter(1))]
        else:
            if sys.version < "3.0":
                return sorted(self._getPeakFamilies(toBytes(nameOrVector), energy), key=itemgetter(1))
            else:
                return [(toString(x[0]), x[1]) for x in \
                        sorted(self._getPeakFamilies(toBytes(nameOrVector), energy), key=itemgetter(1))]

    def _getPeakFamilies(self, std_string name, double energy):
        return self.thisptr.getPeakFamilies(name, energy)

    def _getPeakFamiliesFromVectorOfElements(self, std_vector[std_string] elementList, double energy):
        return self.thisptr.getPeakFamilies(elementList, energy)

    def getBindingEnergies(self, elementName):
        if sys.version < "3.0":
            return self.thisptr.getBindingEnergies(elementName)
        else:
            return toStringKeys(self.thisptr.getBindingEnergies(toBytes(elementName)))

    def getEscape(self, std_map[std_string, double] composition, double energy, double energyThreshold=0.010,
                                        double intensityThreshold=1.0e-7,
                                        int nThreshold=4 ,
                                        double alphaIn=90.,
                                        double thickness=0.0):
        return self.thisptr.getEscape(composition, energy, energyThreshold, intensityThreshold, nThreshold,
                                      alphaIn, thickness)

    def getShellConstants(self, elementName, subshell):
        if sys.version < "3.0":
            return self.thisptr.getShellConstants(elementName, subshell)
        else:
            return toStringKeys(self.thisptr.getShellConstants(toBytes(elementName), toBytes(subshell)))

    def getRadiativeTransitions(self, elementName, subshell):
        if sys.version < "3.0":
            return self.thisptr.getRadiativeTransitions(elementName, subshell)
        else:
            return toStringKeys(self.thisptr.getRadiativeTransitions(toBytes(elementName), toBytes(subshell)))

    def getNonradiativeTransitions(self, elementName, subshell):
        if sys.version < "3.0":
            return self.thisptr.getNonradiativeTransitions(elementName, subshell)
        else:
            return toStringKeys(self.thisptr.getNonradiativeTransitions(toBytes(elementName), toBytes(subshell)))

    def setElementCascadeCacheEnabled(self, elementName, int flag = 1):
        self.thisptr.setElementCascadeCacheEnabled(toBytes(elementName), flag)

    def emptyElementCascadeCache(self, elementName):
        self.thisptr.emptyElementCascadeCache(toBytes(elementName))

    def removeMaterials(self):
        self.thisptr.removeMaterials()
