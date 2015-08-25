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

#include "checkers/BreaksInLoopChecker.hxx"

namespace slint
{

void BreaksInLoopChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isForExp() || e.isWhileExp())
    {
        stack.emplace(&e, 0, 0);
    }
    else if (!stack.empty())
    {
        auto & tuple = stack.top();
        if (e.isBreakExp())
        {
            int & _breaks = std::get<1>(tuple);
            if (_breaks >= 0)
            {
                if (_breaks == maxBreaks)
                {
                    result.report(context, e.getLocation(), L"Number of break is limited: %d max.", maxBreaks);
                    _breaks = -1;
                }
                else
                {
                    ++_breaks;
                }
            }
        }
        else if (e.isContinueExp())
        {
            int & _continues = std::get<2>(tuple);
            if (_continues >= 0)
            {
                if (_continues == maxContinues)
                {
                    result.report(context, e.getLocation(), L"Number of break is limited: %d max.", maxContinues);
                    _continues = -1;
                }
                else
                {
                    ++_continues;
                }
            }
        }
    }
}

void BreaksInLoopChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isForExp() || e.isWhileExp())
    {
        stack.pop();
    }
}

const std::wstring BreaksInLoopChecker::getName() const
{
    return L"BreaksInLoopChecker";
}
}
