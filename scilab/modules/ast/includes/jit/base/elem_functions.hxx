/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __BASE_ELEM_FUNCTIONS_HXX__
#define __BASE_ELEM_FUNCTIONS_HXX__

#include <cmath>
#include <complex>

#include "Cast.hxx"

#include "core_math.h"
#include "faddeeva.h"

extern "C"
{
#include "basic_functions.h"
}

namespace jit
{
    namespace elem
    {

	template<typename T, typename U>
        inline U cast(T x)
        {
            return (U)x;
        }

        template<typename T>
        inline T abs(T x)
        {
	    if (std::is_unsigned<T>::value)
	    {
		return x;
	    }
            return x >= 0 ? x : -x;
        }

        inline double abs(double x)
        {
            return std::fabs(x);
        }

        inline double abs(const std::complex<double> & z)
        {
            return std::hypot(z.real(), z.imag());
        }

        template<typename T>
        inline T ceil(T x)
        {
            return x;
        }

        inline double ceil(double x)
        {
            return std::ceil(x);
        }

        inline std::complex<double> ceil(const std::complex<double> & z)
        {
            return std::complex<double>(std::ceil(z.real()), std::ceil(z.imag()));
        }

        template<typename T>
        inline T floor(T x)
        {
            return x;
        }

        inline double floor(double x)
        {
            return std::floor(x);
        }

        inline std::complex<double> floor(const std::complex<double> & z)
        {
            return std::complex<double>(std::floor(z.real()), std::floor(z.imag()));
        }

        template<typename T>
        inline T trunc(T x)
        {
            return x;
        }

        inline double trunc(double x)
        {
            return std::trunc(x);
        }

        inline std::complex<double> trunc(const std::complex<double> & z)
        {
            return std::complex<double>(std::trunc(z.real()), std::trunc(z.imag()));
        }

        template<typename T>
        inline T round(T x)
        {
            return x;
        }

        inline double round(double x)
        {
            return std::round(x);
        }

        inline std::complex<double> round(const std::complex<double> & z)
        {
            return std::complex<double>(std::round(z.real()), std::round(z.imag()));
        }

        template<typename T>
        inline T min(T x, T y)
        {
            return x < y ? x : y;
        }

        template<typename T>
        inline T max(T x, T y)
        {
            return x > y ? x : y;
        }

        inline double rand()
        {
            static int * _iVal = 0;
            return durands(_iVal);
        }

        template<typename T>
        inline T sign(T x)
        {
            if (std::is_unsigned<T>::value)
            {
                return 1;
            }
            else
            {
                return x < 0 ? -1 : 1;
            }
        }

        inline double sign(double x)
        {
            if (x == 0 || std::isnan(x))
            {
                return x;
            }
            return x < 0 ? -1 : 1;
        }

        inline std::complex<double> sign(const std::complex<double> & z)
        {
            return std::complex<double>(sign(z.real()), sign(z.imag()));
        }

        inline double log2(const double x)
        {
            return std::log(x) / M_LN2;
        }

        inline std::complex<double> sqrt_neg(const double x)
        {
            return std::complex<double>(0, std::sqrt(-x));
        }

        inline std::complex<double> log_neg(const double x)
        {
            return std::complex<double>(std::log(-x), M_PI);
        }

        inline std::complex<double> log10_neg(const double x)
        {
            return std::complex<double>(std::log10(-x), M_PI);
        }

        inline std::complex<double> log2_neg(const double x)
        {
            return std::complex<double>(log2(-x), M_PI);
        }

        inline double erf(const double x)
        {
            return std::erf(x);
        }
	
        inline std::complex<double> erf(const std::complex<double> & z)
        {
            return Faddeeva::erf(z, 0);
        }

        inline double erfc(const double x)
        {
            return std::erfc(x);
        }
	
        inline std::complex<double> erfc(const std::complex<double> & z)
        {
            return Faddeeva::erfc(z, 0);
        }

        inline double erfi(const double x)
        {
            return Faddeeva::erfi(x);
        }
	
        inline std::complex<double> erfi(const std::complex<double> & z)
        {
            return Faddeeva::erfi(z, 0);
        }

        inline double erfcx(const double x)
        {
            return Faddeeva::erfcx(x);
        }

        inline std::complex<double> erfcx(const std::complex<double> & z)
        {
            return Faddeeva::erfcx(z, 0);
        }

        inline double dawson(const double x)
        {
            return Faddeeva::Dawson(x);
        }
	
        inline std::complex<double> dawson(const std::complex<double> & z)
        {
            return Faddeeva::Dawson(z, 0);
        }

        inline double real(const double x)
        {
            return x;
        }
	
        inline double real(const std::complex<double> & z)
        {
            return z.real();
        }

        inline double imag(const double x)
        {
            return 0;
        }
	
        inline double imag(const std::complex<double> & z)
        {
            return z.imag();
        }

        template<typename T>
        inline T identity(T x)
        {
            return x;
        }
	
	inline bool is_strict_negative(const double x)
        {
            return x < 0;
        }
	
    } // namespace BaseFunctions

} // namespace jit

#endif // __BASE_ELEM_FUNCTIONS_HXX__
