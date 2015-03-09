/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __VAR_DEF_HXX__
#define __VAR_DEF_HXX__

#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace analysis
{

/**
 * \struct VarExp
 * \brief Represents a variable with an exponent
 *
 * This struct is mainly used in MultivariateMonomial.
 */
struct VarExp
{
    unsigned long long var;

    // this field is mutable since it is not used in the hash computation
    // and could me modified.
    mutable unsigned int exp;

    /**
     * \brief constructor
     * \param _var the var number
     * \param _exp the exponent
     */
    VarExp(unsigned long long _var, unsigned int _exp = 1) : var(_var), exp(_exp) { }

    /**
     * \brief Print this VarExp
     * \param vars a map containing association between var number and wstring
     * \return a wstring containing the representation of this VarExp
     */
    inline const std::wstring print(const std::map<unsigned long long, std::wstring> & vars) const
    {
        std::wostringstream wos;
        const auto i = vars.find(var);
        if (i != vars.end())
        {
            wos << i->second;
        }
        else
        {
            wos << L"$" << var;
        }

        if (exp > 1)
        {
            wos << L"^" << exp;
        }
        return wos.str();
    }

    /**
     * \brief Overload of the operator <<
     */
    friend inline std::wostream & operator<<(std::wostream & out, const VarExp & ve)
    {
        out << (char)('a' + ve.var);
        if (ve.exp > 1)
        {
            out << L"^" << ve.exp;
        }
        return out;
    }

    /**
     * \brief Overload of the operator <<
     */
    inline bool operator==(const VarExp & R) const
    {
        return var == R.var && exp == R.exp;
    }

    /**
     * \struct Hash
     * \brief Helper struct to be used in unordered container
     */
    struct Hash
    {
        inline std::size_t operator()(const VarExp & ve) const
        {
            return ve.var;
        }
    };

    /**
     * \struct Eq
     * \brief Helper struct to be used in unordered container
     */
    struct Eq
    {
        inline bool operator()(const VarExp & L, const VarExp & R) const
        {
            return L.var == R.var;
        }
    };

    /**
     * \struct Compare
     * \brief Helper struct to be used in map or set
     */
    struct Compare
    {
        inline bool operator()(const VarExp & L, const VarExp & R) const
        {
            return L.var < R.var;
        }
    };
};

} // namespace analysis

#endif // __VAR_DEF_HXX__
