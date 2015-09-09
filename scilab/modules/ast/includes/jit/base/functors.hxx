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

#ifndef __JIT_FUNCTORS_HXX__
#define __JIT_FUNCTORS_HXX__

#include "elem_functions.hxx"
#include "tools.hxx"

namespace jit
{
    namespace functors
    {
	template<typename T, typename U, U (F)(T)>
	struct Unary
	{

	    const static bool as_usual = true; 
	    
	    template<typename V, typename W>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
		{
		    o[i] = F(x[i]);
		}
	};

	template<bool (USECPX)(double), jit::cpx_t (CPX)(double), double (RE)(double)>
	struct Unary_cpx
	{
	    const static bool as_usual = false; 
	    
	    template<typename V>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
		{
		    const double xi = (double)x[i];
		    if (USECPX(xi))
		    {
			o.initIm(size);
			o[i] = CPX(xi);
		    }
		    else
		    {
			o[i] = RE(xi);
		    }
		}
	};

	template<typename T, typename U>
	using sin = Unary<T, U, std::sin>;

	template<typename T, typename U>
	using cos = Unary<T, U, std::cos>;

	template<typename T, typename U>
	using tan = Unary<T, U, std::tan>;

	template<typename T, typename U>
	using tanh = Unary<T, U, std::tanh>;

	template<typename T, typename U>
	using sinh = Unary<T, U, std::sinh>;

	template<typename T, typename U>
	using cosh = Unary<T, U, std::cosh>;

	template<typename T, typename U>
	using atan = Unary<T, U, std::atan>;

	template<typename T, typename U>
	using abs = Unary<T, U, jit::elem::abs>;

	template<typename T, typename U>
	using exp = Unary<T, U, std::exp>;

	template<typename T, typename U>
	using sign = Unary<T, U, jit::elem::sign>;

	template<typename T, typename U>
	using gamma = Unary<T, U, std::tgamma>;

	template<typename T, typename U>
	using gammaln = Unary<T, U, std::lgamma>;

	template<typename T, typename U>
	using ceil = Unary<T, U, jit::elem::ceil>;

	template<typename T, typename U>
	using floor = Unary<T, U, jit::elem::floor>;

	template<typename T, typename U>
	using round = Unary<T, U, jit::elem::round>;

	template<typename T, typename U>
	using trunc = Unary<T, U, jit::elem::trunc>;

	template<typename T, typename U>
	using fix = Unary<T, U, jit::elem::trunc>;

	template<typename T, typename U>
	using int8 = Unary<T, U, jit::elem::cast>;

	template<typename T, typename U>
	using int16 = Unary<T, U, jit::elem::cast>;

	template<typename T, typename U>
	using int32 = Unary<T, U, jit::elem::cast>;

	template<typename T, typename U>
	using int64 = Unary<T, U, jit::elem::cast>;

	template<typename T, typename U>
	using uint8 = Unary<T, U, jit::elem::cast>;

	template<typename T, typename U>
	using uint16 = Unary<T, U, jit::elem::cast>;

	template<typename T, typename U>
	using uint32 = Unary<T, U, jit::elem::cast>;

	template<typename T, typename U>
	using uint64 = Unary<T, U, jit::elem::cast>;

	template<typename T, typename U>
	using erfi = Unary<T, U, jit::elem::erfi>;

	template<typename T, typename U>
	using erfcx = Unary<T, U, jit::elem::erfcx>;

	template<typename T, typename U>
	using erf = Unary<T, U, jit::elem::erf>;

	template<typename T, typename U>
	using erfc = Unary<T, U, jit::elem::erfc>;

	template<typename T, typename U>
	using real = Unary<T, U, jit::elem::real>;

	template<typename T, typename U>
	using imag = Unary<T, U, jit::elem::imag>;
	
	template<typename T, typename U>
	struct log
	{
	    typedef typename jit::functors::Unary<T, U, std::log> F;
	    const static bool as_usual = F::as_usual; 
	    
	    template<typename V, typename W>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
		{
		    F()(std::move(x), std::move(o), i);
		}   
	};

	template<>
	struct log<double, jit::cpx_t>
	{
	    typedef typename jit::functors::Unary_cpx<jit::elem::is_strict_negative, jit::elem::log_neg, std::log> F;
	    const static bool as_usual = F::as_usual; 
	    
	    template<typename V>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
		{
		    F()(std::move(x), std::move(o), i, size);
		}   
	};

	template<typename T, typename U>
	struct log2
	{
	    typedef typename jit::functors::Unary<T, U, jit::elem::log2> F;
	    const static bool as_usual = F::as_usual; 
	    
	    template<typename V, typename W>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
		{
		    F()(std::move(x), std::move(o), i);
		}   
	};

	template<>
	struct log2<double, jit::cpx_t>
	{
	    typedef typename jit::functors::Unary_cpx<jit::elem::is_strict_negative, jit::elem::log2_neg, jit::elem::log2> F;
	    const static bool as_usual = F::as_usual;
	    
	    template<typename V>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
		{
		    F()(std::move(x), std::move(o), i, size);
		}   
	};

	template<typename T, typename U>
	struct log10
	{
	    typedef typename jit::functors::Unary<T, U, std::log10> F;
	    const static bool as_usual = F::as_usual;
	    
	    template<typename V, typename W>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
		{
		    F()(std::move(x), std::move(o), i);
		}   
	};

	template<>
	struct log10<double, jit::cpx_t>
	{
	    typedef typename jit::functors::Unary_cpx<jit::elem::is_strict_negative, jit::elem::log10_neg, std::log10> F;
	    const static bool as_usual = F::as_usual;
	    
	    template<typename V>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
		{
		    F()(std::move(x), std::move(o), i, size);
		}   
	};

	template<typename T, typename U>
	struct sqrt
	{
	    typedef typename jit::functors::Unary<T, U, std::sqrt> F;
	    const static bool as_usual = F::as_usual;
	    
	    template<typename V, typename W>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
		{
		    F()(std::move(x), std::move(o), i);
		}   
	};

	template<>
	struct sqrt<double, jit::cpx_t>
	{
	    typedef typename jit::functors::Unary_cpx<jit::elem::is_strict_negative, jit::elem::sqrt_neg, std::sqrt> F;
	    const static bool as_usual = F::as_usual;
	    
	    template<typename V>
	    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
		{
		    F()(std::move(x), std::move(o), i, size);
		}   
	};
	
    } // namespace functors

} // namespace jit

#endif // __JIT_FUNCTORS_HXX__
