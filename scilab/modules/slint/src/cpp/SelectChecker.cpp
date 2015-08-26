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

#include "checkers/SelectChecker.hxx"

namespace slint
{

void SelectChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::SelectExp & se = static_cast<const ast::SelectExp &>(e);
    if (!se.hasDefault())
    {
        result.report(context, e.getLocation(), L"Select should have a default case.");
    }
    const ast::exps_t cases = se.getCases();
    if (cases.size() == 0)
    {
        result.report(context, e.getLocation(), L"Select with no case.");
    }
    else if (cases.size() == 1)
    {
        result.report(context, e.getLocation(), L"Select with one case: it could be replace by a if statement.");
    }
    else
    {
        enum Type { STRING, NUMBER, OTHER };
        const ast::Exp & first = *static_cast<const ast::CaseExp *>(cases.front())->getTest();
        Type type;
        if (first.isStringExp())
        {
            type = STRING;
        }
        else if (first.isDoubleExp())
        {
            type = NUMBER;
        }
        else
        {
            type = OTHER;
            result.report(context, first.getLocation(), L"Case expression should be a number or string.");
        }

        if (type != OTHER)
        {
            for (ast::exps_t::const_iterator i = std::next(cases.begin()), end = cases.end(); i != end; ++i)
            {
                const ast::Exp & test = *static_cast<const ast::CaseExp *>(*i)->getTest();
                if (type == STRING)
                {
                    if (!test.isStringExp())
                    {
                        result.report(context, test.getLocation(), L"Case expression types are mixed.");
                        break;
                    }
                }
                else if (type == NUMBER)
                {
                    if (!test.isDoubleExp())
                    {
                        result.report(context, test.getLocation(), L"Case expression types are mixed.");
                        break;
                    }
                }
            }
        }
    }
}

void SelectChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::wstring SelectChecker::getName() const
{
    return L"SelectChecker";
}
}
