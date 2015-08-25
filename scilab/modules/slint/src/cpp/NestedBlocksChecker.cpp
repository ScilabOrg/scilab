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

#include "checkers/NestedBlocksChecker.hxx"

namespace slint
{

void NestedBlocksChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    stack.emplace_back(&e);
    if (stack.size() > max)
    {
        result.report(context, stack.front()->getLocation(), L"Too many nested blocks: %d max.", max);
    }
}

void NestedBlocksChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (!stack.empty())
    {
        stack.erase(std::prev(stack.end()));
    }
}

const std::wstring NestedBlocksChecker::getName() const
{
    return L"NestedBlocksChecker";
}
}
