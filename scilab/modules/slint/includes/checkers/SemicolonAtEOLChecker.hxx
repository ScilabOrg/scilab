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

#ifndef __LINT_SEMICOLON_AT_EOL_CHECKER_HXX__
#define __LINT_SEMICOLON_AT_EOL_CHECKER_HXX__

#include "SLintChecker.hxx"
#include "PCREMatcher.hxx"

namespace slint
{

class SemicolonAtEOLChecker : public SLintChecker
{

    PCREMatcher matcher;

public:

    SemicolonAtEOLChecker() : matcher(L"[\\t ]*;[\\t ]*$") { }
    ~SemicolonAtEOLChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::wstring getName() const;

private:

    void check(const ast::Exp * e, SLintContext & context, SLintResult & result) const;
};

} // namespace slint

#endif // __LINT_SEMICOLON_AT_EOL_CHECKER_HXX__
