#ifndef FISX_ELEMENTS_H
#define FISX_ELEMENTS_H
/*!
  \file fisx_elements.h
  \brief Elements properties
  \author V.A. Sole
  \version 0.1
 */

#include <string>
#include <vector>
#include <map>
#include "fisx_simplespecfile.h"
#include "fisx_element.h"
#include "fisx_epdl97.h"
#include "fisx_material.h"

namespace fisx
{

/*!
  \class Elements
  \brief Class handling the physical properties

   This class initializes a default library of physical properties and allows the user
   to modify and to access those properties.
 */
class Elements
{

public:
    /*!
    Initialize the library from the EPDL97 data files found in the provided directory.
    */
    Elements(std::string dataDirectory);

    /*!
    Initialize the library from the EPDL97 data files found in the provided directory.
    It forces the EPDL97 photon photoelectric cross sections to follow the shell binding
    energies found in the provided binding energies file (full path needed because it is
    not necessarily found in the the same directory as the EPDL97 data).
    */
    Elements(std::string dataDirectory, std::string bindingEnergiesFile);

    /*!
    Convenience constructor. It is equivalent to use the previous constructor and
    - to call the setMassAttenuationCoefficientsFile with that file name.
    - to call the setMassAttenuationCoefficientes method with the values extracted from the crossSectionFile.
    */
    Elements(std::string dataDirectory, std::string bindingEnergiesFile, std::string crossSectionsFile);

    // Direct element handling
    /*!
    Returns true if the element with name elementName is already defined in the library.
    */
    bool isElementNameDefined(const std::string & elementName) const;

    /*!
    Returns a reference to the element with name elementName if defined in the library.
    */
    const Element & getElement(const std::string & elementName) const;

    /*!
    Get a copy of the element with name elementName.
    */
    Element getElementCopy(const std::string & elementName);

    // function to ADD or REPLACE if already existing an element
    /*!
    Add an element instance to the library.
    */
    void addElement(const Element & elementInstance);

    /*!
    Retrieve the names of the elements already defined in the library.
    */
    std::vector<std::string> getElementNames();


    /*!
    Convenience method to simplify access to element properties from binding (ex. python)
    */
    const std::map<std::string, double> & getNonradiativeTransitions(const std::string & elementName, \
                                                                     const std::string & subshell) const;

    /*!
    Convenience method to simplify access to element properties from binding (ex. python)
    */
    const std::map<std::string, double> & getRadiativeTransitions(const std::string & elementName, \
                                                                  const std::string & subshell) const;

    // shell related functions
    /*!
    Convenience method to simplify access to element properties from binding (ex. python)
    */
    std::map<std::string, double> getShellConstants(const std::string & elementName, \
                                                    const std::string & subshell) const;

    /*!
    Load main shell (K, L or M) constants from file (fluorescence and Coster-Kronig yields)
    */
    void setShellConstantsFile(const std::string & mainShellName, const std::string & fileName);

    /*!
    Load main shell (K, L or M) X-ray emission rates from file.
    The library normalizes internally.
    */
    void setShellRadiativeTransitionsFile(const std::string & mainShellName, \
                                          const std::string & fileName);

    /*!
    Load main shell (K, L or M) Coster-Kronig and Auger emission ratios from file.
    The library separates Coster-Kronig from Auger and normalizes internally.
    */
    void setShellNonradiativeTransitionsFile(const std::string & mainShellName, \
                                             const std::string & fileName);



    /*!
    Get file used to load main shell (K, L or M) constants (fluorescence and Coster-Kronig yields)
    */
    const std::string &  getShellConstantsFile(const std::string & mainShellName) const;

    /*!
    Get file used to load main shell (K, L or M) X-ray emission rates from file.
    The library normalizes internally.
    */
    const std::string &  getShellRadiativeTransitionsFile(const std::string & mainShellName) const;

    /*!
    Get file used to load main shell (K, L or M) Coster-Kronig and Auger emission ratios from file.
    The library separates Coster-Kronig from Auger and normalizes internally.
    */
    const std::string & getShellNonradiativeTransitionsFile(const std::string & mainShellName) const;

    // mass attenuation related functions
    /*!
    Update the total mass attenuation coefficients of the default elements with those found
    in the given file.
    */
    void setMassAttenuationCoefficientsFile(const std::string & fileName);

    /*!
    Update the total mass attenuation coefficients of the supplied element.
    The partial mass attenuation photoelectric coefficients are updated by the library in order
    to be consistent with the supplied mass attenuation coefficients
    */
    void setMassAttenuationCoefficients(const std::string & elementName, \
                                        const std::vector<double> & energy, \
                                        const std::vector<double> & photoelectric, \
                                        const std::vector<double> & coherent, \
                                        const std::vector<double> & compton,\
                                        const std::vector<double> & pair);

    /*!
    Retrieve the internal table of photon mass attenuation coefficients of the requested element.
    */
    std::map<std::string, std::vector<double> > getMassAttenuationCoefficients( \
                                                        const std::string & elementName) const;

    /*!
    Given an element or formula and a set of energies, give back the mass attenuation coefficients
    at the given energies as a map where the keys are the different physical processes and the values
    are the vectors of the calculated values via log-log interpolation in the internal table.
    */
    std::map<std::string, std::vector<double> > getMassAttenuationCoefficients( \
                                                    const std::string & formula,
                                                    const std::vector<double> & energies) const;

    /*!
    Given a map of elements and mass fractions and a set of energies, give back the mass attenuation
    coefficients at the given energies as a map where the keys are the different physical processes and the
    values are the vectors of the calculated values via log-log interpolation in the internal table.
    */
    std::map<std::string, std::vector<double> > getMassAttenuationCoefficients(\
                                                std::map<std::string, double> elementMassFractions,\
                                                std::vector<double> energies) const;


    /*!
    Convenience method.
    Given an element or formula and an energy, give back the mass attenuation coefficients at the
    given energy as a map where the keys are the different physical processes and the values are
    calculated values via log-log interpolation in the internal table.
    */
    std::map<std::string, double> getMassAttenuationCoefficients(std::string formula, double energy) const;

    /*!
    Convenience method.
    Given a map of elements and mass fractions element and one energy, give back the mass attenuation
    coefficients at the given energy as a map where the keys are the different physical processes and the
    values are the calculated values via log-log interpolation in the internal table.    */
    std::map<std::string, double> getMassAttenuationCoefficients(\
                                                std::map<std::string, double> elementMassFractions,\
                                                double energies) const;

    // Material handling
    /*!
    Create a new Material given name and initialize its density, thickness and comment.
    The material is *not* added to the internal list of materials.
    */
    Material createMaterial(const std::string & name, const double & density = 1.0,
                            const double & thickness = 1.0, const std::string  & comment = "");

    /*!
    Set the material composition of the material with name materialName.
    The material must belong to the internal list defined materials.
    A composition is a map where the keys are elements/materials already defined in the library
    and the values are mass amounts.
    The library is supposed to normalize to make sure the mass fractions sum unity.
    */
    void setMaterialComposition(const std::string & materialName, \
                                const std::map<std::string, double> & composition);

    /*!
    Set the material composition of the material with name materialName.
    It is composed of the elements/materials with the given names and mass amounts.
    The library is supposed to normalize to make sure the mass fractions sum unity.
    */
    void setMaterialComposition(const std::string & materialName, \
                                const std::vector<std::string> & names,\
                                const std::vector<double> & amounts);

    /*!
    Retrieve a reference to instance of the material identified by materialName
    */
    const Material & getMaterial(const std::string & materialName);

    /*!
    Retrieve a copy of the instance of the material identified by materialName
    */
    Material getMaterialCopy(const std::string & materialName);

    /*!
    Copy a material into the set of defined materials.
    */
    void addMaterial(const Material & materialInstance, const int & errorOnReplace = 1);

    /*!
    Create and add Material instance to the set of defined materials.
    */
    void addMaterial(const std::string & name, const double & density = 1.0, \
                        const double & thickness = 1.0, const std::string  & comment = "", \
                        const int & errorOnReplace = 1);

    /*!
    Remove a material (if present) from the set of defined materials
    */
    void removeMaterial(const std::string & name);

    /*!
    Empty the list of defined materials
    */
    void removeMaterials();

    /*!
    Try to interpret a given string as a formula, returning the associated mass fractions
    as a map of elements and mass fractions. In case of failure, it returns an empty map.
    */
    std::map<std::string, double> getCompositionFromFormula(const std::string & formula) const;

    /*!
    Try to interpret a given string as a chemical formula or a defined material, returning the
    associated mass fractions as a map of elements and mass fractions.
    In case of failure, it returns an empty map.
    */
    std::map<std::string, double> getComposition(const std::string & name) const;

    /*!
    Try to parse a given string as a formula, returning the associated number of "atoms"
    per single molecule. In case of failure, it returns an empty map.
    */
    std::map<std::string, double> parseFormula(const std::string & formula) const;


    /*!
    Given a set of energies and (optional) weights, for the specfified element, this method returns
    the emitted X-rays already corrected for cascade and fluorescence yield.
    It is the equivalent of the excitation factor in D.K.G. de Boer's paper.
    */
    std::vector<std::map<std::string, std::map<std::string, double> > > getExcitationFactors( \
                            const std::string & element,
                            const std::vector<double> & energy,
                            const std::vector<double> & weights = std::vector<double>()) const;

    /*!
    Given an energy and its (optional) weight, for the specfified element, this method returns
    the emitted X-ray already corrected for cascade and fluorescence yield.
    It is the equivalent of the excitation factor in D.K.G. de Boer's paper.
    */
    std::map<std::string, std::map<std::string, double> > getExcitationFactors( \
                            const std::string & element,
                            const double & energy, \
                            const double & weights = 1.0) const;

    /*!
    Given an element, formula or material return an ordered vector of pairs. The first element
    is the peak family ("Si K", "Pb L1", ...) and the second the binding energy.
    */
    std::vector<std::pair<std::string, double> > getPeakFamilies(const std::string & name, \
                                                 const double & energy) const;


    /*!
    Given a list of elements return an ordered vector of pairs. The first element
    is the peak family ("Si K", "Pb L1", ...) and the second the binding energy.
    */
    std::vector<std::pair<std::string, double> > getPeakFamilies(const std::vector<std::string> & elementList, \
                                             const double & energy) const;

    /*!
    Convenience function to simplify python use ...
    */
    const std::map<std::string, double> & getBindingEnergies(const std::string & name) const;

    /*!
    Return escape peak energy and rate per detected photon of given energy.
    Given a detector composition and an incident energy in keV, gives back a map of the form:
    result["element_transitionesc"]["energy"]
    result["element_transitionesc"]["rate"]
    For instance, if the incident energy is 5 keV and the composition is Si, the output
    would have the form:

    result["Si_KL3esc"] ["energy"] = 5.0 - Si KL3 transition energy
    result["Si_KM3esc"] ["energy"] = 5.0 - Si KM3 transition energy
    ...

    The rest of (optional) parameters condition the output as follows:

    energyThreshold - Separation between two lines to be considered together. Default 0.010 keV.
    intensityThreshold - Minimum absolute peak intensity to consider. Default 1.0e-7
    nThreshold - Maximum number of escape peaks to consider. Default is 4.
    alphaIn - Incoming beam angle with detector surface. Default 90 degrees.
    thickness - Material thickness in g/cm2. Default is 0 to indicate infinite thickness

    */
    std::map<std::string, std::map<std::string, double> > getEscape(const std::map<std::string, double> & composition, \
                                        const double & energy, \
                                        const double & energyThreshold = 0.010, \
                                        const double & intensityThreshold = 1.0e-7, \
                                        const int & nThreshold = 4 , \
                                        const double & alphaIn = 90.,\
                                        const double & thickness = 0.0) const;



    /*!
    Optimization methods to keep the complete emission cascade following a single vacancy in a shell
    into cache.
    */
    void setElementCascadeCacheEnabled(const std::string & elementName, const int & flag = 1);
    int isElementCascadeCacheFilled(const std::string & elementName) const;
    void fillElementCascadeCache(const std::string & elementName);
    void emptyElementCascadeCache(const std::string & elementName);

    /*!
    Utility to convert from string to double.
    */
    static bool stringToDouble(const std::string & str, double& number);

    /*!
    Utility to convert from double to string.
    */
    static std::string toString(const double& number);

private:

    void initialize(std::string, std::string);

    // The EPDL97 library
    EPDL97 epdl97;

    // The map of defined elements
    // The map has the form Element Name, Index
    std::map<std::string , int> elementDict;

    // The vector of defined elements
    std::vector<Element> elementList;

    // The vector of defined Materials
    std::vector<Material> materialList;

    // Utility function
    const std::vector<Material>::size_type getMaterialIndexFromName(const std::string & name) const;

    // The files used for configuring the library
    std::map<std::string, std::string> shellConstantsFile;
    std::map<std::string, std::string> shellRadiativeTransitionsFile;
    std::map<std::string, std::string> shellNonradiativeTransitionsFile;

    struct sortVectorOfExcited {
        bool operator()(const std::pair<std::string, double> &left, const std::pair<std::string,int> &right) {
        return left.second < right.second;
    }
};

};

} // namespace fisx

#endif //FISX_ELEMENTS_H
