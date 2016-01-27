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

#ifndef __COMPLETE_MACRO_SIGNATURE_HXX__
#define __COMPLETE_MACRO_SIGNATURE_HXX__

#include <iostream>
#include <set>
#include <unordered_set>

#include "alltypes.hxx"
#include "allexp.hxx"
#include "MacroDef.hxx"
#include "MacroSignature.hxx"
#include "TITypeTuple.hxx"
#include "TITypeSignatureTuple.hxx"
#include "dynlib_ast.h"

namespace analysis
{

struct MacroOut
{
    TITypeTuple tuple;
    int maxVarId;
};

class AnalysisVisitor;
class DataManager;

/**
 * \struct CompleteMacroSignature
 * \brief a MacroSignature is not enought to determinate a macro
 * Since a macro can use variables not defined in its scope and since constraints can be applied to arguments
 * we need to have a more complete signature.
 */
class CompleteMacroSignature
{
    struct __MPCMO
    {
	uint64_t id;
        MPolyConstraintSet verified;
	ConstraintManager::UnverifiedSet unverified;
	std::set<symbol::Symbol> globalConstants;
        MacroOut out;

        __MPCMO(const uint64_t _id, const MPolyConstraintSet & _verified, const ConstraintManager::UnverifiedSet & _unverified, const std::set<symbol::Symbol> & _globalConstants, MacroOut && _out) : id(_id), verified(_verified), unverified(_unverified), globalConstants(_globalConstants), out(_out) { }

	inline std::size_t getUnverifiedSize() const
	    {
		std::size_t size = 0;
		for (const auto & mpcs : unverified)
		{
		    size += mpcs.size();
		}
		return size;
	    }
	
	struct Compare
	{
	    inline bool operator()(const __MPCMO & L, const __MPCMO & R) const
		{
		    std::size_t sizeL = L.getUnverifiedSize();
		    std::size_t sizeR = R.getUnverifiedSize();
		    if (sizeL < sizeR)
		    {
			return true;
		    }
		    else if (sizeL == sizeR)
		    {
			sizeL = L.verified.size();
			sizeR = R.verified.size();
			if (sizeL < sizeR)
			{
			    return true;
			}
			else if (sizeL == sizeR)
			{
			    return L.id < R.id;
			}
		    }
		    return false;
		}
	};
    };

    uint64_t & id;
    // This map contains the signatures corresponding to the global symbols mapped to out types
    std::set<__MPCMO, __MPCMO::Compare> outMap;
    GVN & gvn;

public:

    CompleteMacroSignature(GVN & _gvn, uint64_t & _id) : id(_id), gvn(_gvn) { }

    inline GVN & getGVN()
	{
	    return gvn;
	}

    const MacroOut * getOutTypes(AnalysisVisitor & visitor, const MacroSignature & signature, MacroDef * macrodef, DataManager & dm, const unsigned int rhs, std::vector<TIType> & in, const std::vector<GVN::Value *> values, uint64_t & functionId);

    const MacroOut * analyze(AnalysisVisitor & visitor, const MacroSignature & signature, MacroDef * macrodef, DataManager & dm, const unsigned int rhs, std::vector<TIType> & in, const std::vector<GVN::Value *> values, uint64_t & functionId);

    friend std::wostream & operator<<(std::wostream & out, const CompleteMacroSignature & cms);

};

} // namespace analysis

#endif // __COMPLETE_MACRO_SIGNATURE_HXX__
