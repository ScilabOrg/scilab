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

#ifndef __LINT_TODO_CHECKER_HXX__
#define __LINT_TODO_CHECKER_HXX__

#include "SLintChecker.hxx"
#include "PCREMatcher.hxx"

namespace slint
{

class TodoChecker : public SLintChecker
{

    PCREMatcher todo;

public:

    TodoChecker() : todo(L"[tT][oO][dD][oO][^a-zA-Z0-9]+") { }
    ~TodoChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::wstring getName() const;
};

} // namespace slint

#endif // __LINT_TODO_CHECKER_HXX__
