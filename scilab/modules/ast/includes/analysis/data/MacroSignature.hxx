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

#ifndef __MACRO_SIGNATURE_HXX__
#define __MACRO_SIGNATURE_HXX__

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "tools.hxx"
#include "TITypeSignatureTuple.hxx"

namespace analysis
{

class MacroDef;
class DataManager;

/**
 * \struct MacroSignature
 * \brief A MacroSignature is defined by a macro name, the number of LHS and the types of the arguments
 */
struct MacroSignature
{
    const std::string name;
    const unsigned int lhs;
    TITypeSignatureTuple tuple;

    MacroSignature(MacroDef & macrodef, const unsigned int _lhs, const std::vector<TIType> & in) : name(macrodef.getName()), lhs(_lhs), tuple(in) { }

    friend std::ostream & operator<<(std::ostream & out, const MacroSignature & signature)
    {
        out << "Signature{"
            << "name:" << signature.name
            << ", lhs:" << signature.lhs
            << ", types:" << signature.tuple
            << "}";

        return out;
    }

    /**
     * \struct Hash
     * \brief Hash struct to be used in unordered_* container
     */
    struct Hash
    {
        inline std::size_t operator()(const MacroSignature & sign) const
        {
            return tools::hash_combine(std::hash<std::string>()(sign.name), sign.lhs, sign.tuple.hash());
        }
    };

    /**
     * \struct Eq
     * \brief Eq struct to be used in unordered_* container
     */
    struct Eq
    {
        inline bool operator()(const MacroSignature & L, const MacroSignature & R) const
        {
            return L.lhs == R.lhs && L.name == R.name && L.tuple == R.tuple;
        }
    };
};

} // namespace analysis

#endif // __MACRO_SIGNATURE_HXX__
