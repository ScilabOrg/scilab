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

#ifndef __LINT_VARIABLES_CHECKER_HXX__
#define __LINT_VARIABLES_CHECKER_HXX__

#include <stack>
#include <unordered_map>

#include "SLintChecker.hxx"

namespace slint
{

class VariablesChecker : public SLintChecker
{

    std::stack<std::unordered_map<std::wstring, Location>> assigned;
    std::stack<std::unordered_set<std::wstring>> used;

public:

    VariablesChecker()  { }
    ~VariablesChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::wstring getName() const;
};

} // namespace slint

#endif // __LINT_VARIABLES_CHECKER_HXX__
