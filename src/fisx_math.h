#ifndef FISX_DEBOER_H
#define FISX_DEBOER_H

namespace fisx
{

/*!
  \class Math
  \brief Common fluorescence spectroscopy math functions

   De Boer Formulae for secondary excitation corrections

   This class implements the formulae contained in the article:
   D.K.G. de Boer, X-Ray Spectrometry, Vol. 19, (1990) 145 - 154.
 */

class Math
{
    public:
        /*!
        Calculate the exponential integral of order 1 on x.

                   |inf
                  |          dy
        E1(x) =  |   exp(y) ----
                |x            y

        Only the real part is calculated

        */
        static double E1(const double & x);

        /*!
        Calculate the exponential integral of order n on x

                   |inf
                  |          dy
        En(x) =  |   exp(y) ----
                |x            y^n

        Only the real part is calculated

        */
        static double En(const int & n, const double & x);


        /*!
        Calculates exp(x) * E1(x)
        Used in D.K.G. de Boer, X-Ray Spectrometry, Vol. 19, (1990) 145 - 154
        and needed to calculate secondary excitation.

        Implemented following Abramowitz and Stegun
        */
        static double deBoerD(const double & x);


        /*!
        Calculates the integral part of expression 6 of the article
        D.K.G. de Boer, X-Ray Spectrometry, Vol. 19, (1990) 145 - 154
        and needed to calculate secondary excitation.
        muj is the matrix mass attenuation coefficient at the matrix emitted line j
        mui is the matrix mass attenuation coefficient at the fluorescent line i
        mul is the matrix mass attenuation coefficient at the incident energy
        */
        static double deBoerL0(const double & mu1, const double & mu2, const double & muj, \
                               const double & density, const double & thickness);


        /*!
        For multilayers
        p and q following article
        d1 is the product density * thickness of fluorescing layer
        d2 is the product density * thickness of layer j originating the secondary excitation
        mu_1_j is the mass attenuation coefficient of fluorescing layer at j excitation energy
        mu_2_j is the mass attenuation coefficient of layer j at j excitation energy
        mu_b_d_t is the sum of the products mu * density * thickness of layers between layer i and j
        */
        static double deBoerX(const double & p, const double & q, \
                              const double & d1, const double & d2, \
                              const double & mu_1_j, const double & mu_2_j, \
                              const double & mu_b_j_d_t = 0.0);

        static double deBoerV(const double & p, const double & q, \
                              const double & d1, const double & d2, \
                              const double & mu_1_j, const double & mu_2_j, \
                              const double & mu_b_j_d_t);

        /*!
        Returns false is x is NaN
        */
        static bool isNumber(const double & x);

        /*!
        Returns false is x is NaN or an infinite number
        */
        static bool isFiniteNumber(const double & x);

        /*!
        Help function, see formula A2a appendix
        */
        static double deBoerY(const double & a, const double & b, const double & c, const double & d);

        /*!
        Help function, see formula A2b appendix
        */
        static double deBoerW(const double & a, const double & b, const double & d);

        /*!
        Abramowitz and Stegun formula 5.1.53
        Returns E1(x) + log(x)
        Formula is valid for 0 <= x <= 1
        */
        static double AS_5_1_53(const double & x);

        /*!
        Abramowitz and Stegun formula 5.1.56
        Returns x * exp(x) * E1(x)
        Formula is valid for 1 <= x < +inf
        */
        static double AS_5_1_56(const double & x);

        /*!
        Get FWHM from energy, noise, fano and average energy needed to create a signal
        */
        static double getFWHM(const double & energy, const double & noise, \
                              const double & fano, const double & quantumEnergy = 0.00385);

        /*!
        Error function
        */
        static double erf(const double & x);


        /*!
        Complementary error function
        */
        static double erfc(const double & x);

        /*!
        Evaluate HYPERMET function
        */
        static double hypermet(const double & x,
                               const double & gaussArea, \
                               const double & position, \
                               const double & fwhm, \
                               const double & shortTailArea = 0.0, \
                               const double & shortTailSlope = 0.03, \
                               const double & longTailArea = 0.0, \
                               const double & longTailSlope = 1.0, \
                               const double & stepHeight = 0.0);


    private:
        /*!
        Evaluate exp(x) * E1(x) for x > 1

        This makes use of the continued fraction expandion of E1(x) derived from
        Mathematica web site expression for En(z).

        The modified Lenz algorithm is implemented as described in Numerical
        Recipes.
        */
        static double _deBoerD(const double &x, \
                        const double & epsilon = 1.0e-7, \
                        const int & maxIter = 100);
};

} // namespace fisx

#endif // FISX_DEBOER_H
