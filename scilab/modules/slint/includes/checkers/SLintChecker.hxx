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

#ifndef ___SLINT_CHECKER_HXX__
#define ___SLINT_CHECKER_HXX__

#include <string>
#include <unordered_set>

#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"

#include "SLintContext.hxx"
#include "SLintResult.hxx"

namespace slint
{

class SLintChecker
{

protected:

    static std::unordered_set<std::wstring> constants;

public:

    SLintChecker() { }

    virtual ~SLintChecker() { }

    virtual void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) = 0;
    virtual void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) = 0;
    virtual const std::wstring getName() const = 0;

    static bool isScilabConstant(const std::wstring & name);

private:

    static std::unordered_set<std::wstring> init();
};

} // namespace slint

#endif // __SLINT_CHECKER_HXX__
