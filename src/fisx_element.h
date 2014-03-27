#ifndef FISX_ELEMENT_H
#define FISX_ELEMENT_H
#include <string>
#include <ctype.h>
#include <vector>
#include <map>
#include "fisx_shell.h"
#include "fisx_epdl97.h"

class Element
{
public:
    /*!
    Create a new Element instance.
    It will need calls to setName and setAtomicNumber in order to be able to use other methods.
    */
    Element(); // Do not use
    /*!
    Create a new Element instance of an element with the given name and atomic number.
    It will need calls to setName and setAtomicNumber in order to be able to use other methods.
    This is the expected instantiation method.
    */
    Element(std::string name, int z); //
    // TODO: Element copy constructor to be added in order to be able to generate a new
    // element from a given one and modify some properties

    /*!
    Set element name. It is not limited to two characters. WARNING: An element name should not be
    changed unless we are making a copy from other element in order to change some properties.
    */
    void setName(const std::string & name);

    /*!
    Retrieves the given element name.
    */
    std::string getName();

    /*!
    Set atomic number. It has to be a positive integer. WARNING: An element atomic number not be
    changed unless we are making a copy from other element in order to change some properties.
    */
    void setAtomicNumber(const int & z);
    /*!
    Retrieves the given element atomic number.
    */
    const int & getAtomicNumber() const;

    /*!
    Set the given element atomic number. WARNING: An element atomic mass not be changed unless we are making a
    copy from other element in order to change some properties.
    */
    void setAtomicMass(const double & mass);

    /*!
    Retrieves the given element atomic mass.
    */
    const double & getAtomicMass() const;

    // density (initialized by default to 1.0)
    /*!
    Set the given element density (in g/cm3). Initialized by default to 1.0 g/cm3. WARNING: An already
    changed element density not be modified unless we are making a copy from other element in order to
    change some properties.
    */
    void setDensity(const double &);

    /*!
    Retrieves the given element density.
    */
    double getDensity();

    // binding energies

    /*!
    Set element binding energies (in keV) as a map of doubles whith the keys indicating
    the respective atomic shells: K, L1, L2, L3, M1, ... , M5, N1, ..., N7, and so on.
    */
    void setBindingEnergies(std::map<std::string, double> bindingEnergies);

    /*!
    Convenience method to set the binding energies.
    */
    void setBindingEnergies(std::vector<std::string> labels, std::vector<double> energies);

    /*!
    Retrieves the internal map of binding energies
    */
    const std::map<std::string, double> & getBindingEnergies();

    /*!
    Given a photon energie (in keV) gives back the excited shells
    */
    std::vector<std::string> getExcitedShells(const double & energy);

    // Mass attenuation coefficients

    // This methods overwrites any totals given
    /*!
    Set the photon mass attenuation coefficcients (in cm2/g) of the element at the given
    energies (in keV).
    */
    void setMassAttenuationCoefficients(const std::vector<double> & energies, \
                                        const std::vector<double> & photoelectric, \
                                        const std::vector<double> & coherent, \
                                        const std::vector<double> & incoherent, \
                                        const std::vector<double> & pair);

    /*!
    Convenience method skipping pair production mass attenuation coefficients. They
    will be internaly considered as zero.
    */
    void setMassAttenuationCoefficients(const std::vector<double> & energies, \
                                        const std::vector<double> & photoelectric, \
                                        const std::vector<double> & coherent, \
                                        const std::vector<double> & incoherent);

    /*!
    TODO. Not yet implemented.
    If the this total mass attenuation is supplied, photoelectric effect mass attenuation
    will be defined as this total minus the sum of the other effects. The idea is to be able
    to supply a measured absorption spectrum.
    */
    void setTotalMassAttenuationCoefficient(const std::vector<double> & energies, \
                                            const std::vector<double> & total);

    /*!
    Retrieves the internal table of energies and associated mass attenuation coefficients
    */
    const std::map<std::string, std::vector<double> > & getMassAttenuationCoefficients() const;

    /*!
    Calculates via log-log interpolation in the internal table the mass attenuation coefficients
    at the given set of energies.
    */
    std::map<std::string, std::vector<double> > getMassAttenuationCoefficients(\
                                                const std::vector<double> & energy) const;
    /*!
    Convenience method. Calculates via log-log interpolation in the internal table the mass
    attenuation coefficients at the given energy.
    */
    std::map<std::string, double> getMassAttenuationCoefficients(const double & energy) const;

    std::map<std::string, std::pair<double, int> > extractEdgeEnergiesFromMassAttenuationCoefficients();
    std::map<std::string, std::pair<double, int> > extractEdgeEnergiesFromMassAttenuationCoefficients(\
                                                            const std::vector<double> & energies,\
                                                            const std::vector<double> & muPhotoelectric);

    // Partial shell mass attenuation photoelectric coefficients
    /*!
    Set the photon partial photoelectric cross sections (in cm2/g)  for the given shell name.
    Only the EPDL97 library seems to offer these cross sections.
    */
    void setPartialPhotoelectricMassAttenuationCoefficients(const std::string & shell,\
                                                const std::vector<double> & energy, \
                                                const std::vector<double> & partialPhotoelectric);

    /*!
    Retrieves the internal table of partial photoelectric cross sections (in cm2/g)  at the given energy.
    */
    std::map<std::string, double> getPartialPhotoelectricMassAttenuationCoefficients(\
                                                                    const double & energy) const;

    // Shell transitions description
    void setRadiativeTransitions(std::string subshell, std::map<std::string, double> values);

    void setRadiativeTransitions(std::string subshell,\
                                 std::vector<std::string>,\
                                 std::vector<double> values);
    const std::map<std::string, double> & getRadiativeTransitions(std::string subshell);

    void setNonradiativeTransitions(std::string subshell,
                                    std::vector<std::string>,
                                    std::vector<double> values);

    void setNonradiativeTransitions(std::string subshell,
                                    std::map<std::string, double> values);

    const std::map<std::string, double> & getNonradiativeTransitions(const std::string & subshell);

    // Shell constants (fluorescence yield, Coster-Kronig yields)
    void setShellConstants(std::string subshell, std::map<std::string, double> constants);
    std::map<std::string, double> getShellConstants(std::string subshell);

    std::map<std::string, std::map<std::string, double> > getXRayLines(std::string family);

    /*!
    Given a set of energies, give the initial distribution of vacancies (before cascade) due to
    photoelectric effect.
    The output map keys correspond to the different partial photoelectric shells and the values
    are just vectors of mu_photoelectric(shell, E)/mu_photoelectric(total, E)
    */
    std::map<std::string, std::vector <double> >getInitialPhotoelectricVacancyDistribution(\
                                                const std::vector<double> & energies) const;

    /*!
    Given one energy, give the initial distribution of vacancies (before cascade) due to
    photoelectric effect.
    The output map keys correspond to the different subshells and the values are just
    mu_photoelectric(shell, E)/mu_photoelectric(total, E).
    */
    std::map<std::string, double> getInitialPhotoelectricVacancyDistribution(const double & energy) const;

    std::map<std::string, double> getCascadeModifiedVacancyDistribution(const std::map<std::string, \
                                                                        double> & distribution) const;

    std::map<std::string, std::map<std::string, double> >\
        getXRayLinesFromVacancyDistribution(const std::map<std::string, double> & distribution, \
                                            const int & cascade = 1,
                                            const bool & useFluorescenceYield = true) const;

    /*!
    Given a set of energies and (optional) weights returns the emitted X-ray already
    corrected for cascade and fluorescence yield following photoelectric
    interaction.
    */
    std::vector<std::map<std::string, std::map<std::string, double> > > \
                            getPhotoelectricExcitationFactors( \
                                const std::vector<double> & energy,
                                const std::vector<double> & weights = std::vector<double>()) const;

    /*!
    Given an energy and its (optional) weight, returns the emitted X-ray already
    corrected for cascade and fluorescence yield. If the weight is one, that
    corresponds to the different emitted x-rays per incident photon following photoelectric
    interaction.
    */
    std::map<std::string, std::map<std::string, double> > getPhotoelectricExcitationFactors( \
                                                    const double & energy,
                                                    const double & weight = 1.0) const;


    const Shell & getShell(std::string);

    /*!
    Provide an easier to wrap interface than calling getShell to access important shell functions
    */
    const std::map<std::string, double> & getFluorescenceRatios(std::string subshell);
    const std::map<std::string, double> & getAugerRatios(std::string subshell);
    const std::map<std::string, std::map<std::string, double> > & getCosterKronigRatios(std::string subshell);

    /*!
    Helper to locate interpolation indices.
    */
    std::pair<long, long> getInterpolationIndices(const std::vector<double> &,  const double &) const;

private:
    std::string name;
    int    atomicNumber;
    double density;
    double atomicMass;

    std::map<std::string, double> bindingEnergy;
    // Mass attenuation coefficients and energies
    std::vector<double> muEnergy;
    std::map< std::string, std::vector<double> >mu;

    // Partial photoelectric mass attenuation coefficients
    // For each shell (= key), there is a vector for the energies
    // and a vector for the value of the mass attenuation coefficients
    // Expected map key values are:
    // K, L1, L2, L3, M1, M2, M3, M4, M5, "REST"
    void initPartialPhotoelectricCoefficients();
    std::map<std::string, std::vector<double> > muPartialPhotoelectricEnergy;
    std::map<std::string, std::vector<double> > muPartialPhotoelectricValue;

    // Shell instance to handle cascade
    std::map<std::string, Shell> shellInstance;

    // map of the form {"L2":{"omega": fluorescence_yield,
    //                        "f12": f12,
    //                        "f13": f13}
    // std::map<std::string, std::map<std::string, double> > shellConstants;

    // map of the form {"KL3":{"energy": bindingEnergy["K"] - bindingEnergy["L3"],
    //                         "rate": shellInstance["K"].getFluorescenceRatios()["KL3"]}
    std::map<std::string, std::map<std::string, double> > shellXRayLines;
};
#endif
// FISX_ELEMENT_H
