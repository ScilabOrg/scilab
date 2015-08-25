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

#ifndef __SLINT_SCILAB_RESULT_HXX__
#define __SLINT_SCILAB_RESULT_HXX__

#include <map>
#include <unordered_map>

#include "SLintResult.hxx"

namespace slint
{

class SLintScilabResult : public SLintResult
{

    std::unordered_map<std::wstring, std::multimap<Location, std::wstring>> results;

public:

    virtual ~SLintScilabResult();
    virtual void handleMessage(SLintContext & context, const Location & loc, const std::wstring & msg);
    virtual void getResults() const;
};

} // namespace slint

#endif // __SLINT_SCILAB_RESULT_HXX__
