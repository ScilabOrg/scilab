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

#include "checkers/LoadSaveChecker.hxx"

namespace slint
{
void LoadSaveChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isCallExp())
    {
        const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
        if (ce.getName().isSimpleVar())
        {
            const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
            if (name == L"load" || name == L"save")
            {
                const ast::exps_t args = ce.getArgs();
                if (args.size() == 0)
                {
                    result.report(context, e.getLocation(), *this, _("Function %s have at least one argument."), name);
                }
                else if (args.size() == 1)
                {
                    result.report(context, e.getLocation(), *this, _("Function %s should be called with names of variables."), name);
                }
                else
                {
                    for (ast::exps_t::const_iterator i = std::next(args.begin()), end = args.end(); i != end; ++i)
                    {
                        const ast::Exp & arg = **i;
                        if (!arg.isStringExp())
                        {
                            result.report(context, e.getLocation(), *this, _("Function %s should be called with constant strings."), name);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void LoadSaveChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string LoadSaveChecker::getName() const
{
    return "LoadSaveChecker";
}
}
