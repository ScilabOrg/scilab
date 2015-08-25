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

#include <iostream>

#include "checkers/PrintfChecker.hxx"

namespace slint
{
void PrintfChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    if (ce.getName().isSimpleVar())
    {
        const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
        if (name == L"printf" || name == L"mprintf" || name  == L"sprintf" || name == L"msprintf")
        {
            ast::exps_t args = ce.getArgs();
            if (!args.empty())
            {
                const ast::Exp & first = *args.front();
                if (first.isStringExp())
                {
                    const std::wstring & format = static_cast<const ast::StringExp &>(first).getValue();
                    unsigned int percents = 0;
                    for (std::wstring::const_iterator i = format.begin(), end = format.end(); i != end; ++i)
                    {
                        if (*i == L'%')
                        {
                            if (std::next(i) != end)
                            {
                                if (*std::next(i) != L'%')
                                {
                                    ++percents;
                                }
                                ++i;
                            }
                        }
                    }
                    if (percents != args.size() - 1)
                    {
                        result.report(context, e.getLocation(), L"Invalid number of arguments in %s: data don\'t correspond to the format.", name);
                    }
                }
            }
        }
    }
}

void PrintfChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::wstring PrintfChecker::getName() const
{
    return L"PrintfChecker";
}
}
