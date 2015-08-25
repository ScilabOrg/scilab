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

#include "checkers/VariablesChecker.hxx"

namespace slint
{
void VariablesChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        const ast::FunctionDec & fd = static_cast<const ast::FunctionDec &>(e);
        if (!assigned.empty())
        {
            // we declare the function in the current scope
            assigned.top().emplace(fd.getSymbol().getName(), e.getLocation());
        }

        assigned.emplace(std::unordered_map<std::wstring, Location>());
        used.emplace(std::unordered_set<std::wstring>());

        // a function cans refer to itself
        assigned.top().emplace(fd.getSymbol().getName(), e.getLocation());
    }
    else if (e.isSimpleVar())
    {
        const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e);
        if (context.isAssignedVar(var))
        {
            const std::wstring & name = var.getSymbol().getName();
            assigned.top().emplace(name, var.getLocation());
            used.top().erase(name);
        }
        else if (!e.getParent()->isFieldExp() || static_cast<const ast::FieldExp *>(e.getParent())->getTail() != &e)
        {
            const std::wstring & name = var.getSymbol().getName();
            if (used.top().find(name) == used.top().end())
            {
                used.top().emplace(name);
                if (assigned.top().find(name) == assigned.top().end())
                {
                    if (!context.isFunIn(name) && !SLintChecker::isScilabConstant(name))
                    {
                        types::InternalType * pIT = symbol::Context::getInstance()->get(var.getSymbol());
                        if (!pIT || (!pIT->isFunction() && !pIT->isMacroFile() && !pIT->isMacro()))
                        {
                            result.report(context, e.getLocation(), L"Use of non-initialized variable \'%s\' may have any side-effects.", name);
                        }
                    }
                }
                else
                {
                    assigned.top().erase(name);
                }
            }
        }
    }
    else if (e.isVarDec())
    {
        const ast::VarDec & vd = static_cast<const ast::VarDec &>(e);
        const std::wstring & name = vd.getSymbol().getName();
        assigned.top().emplace(name, vd.getLocation());
        used.top().erase(name);
    }
}

void VariablesChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        const ast::FunctionDec & fd = static_cast<const ast::FunctionDec &>(e);
        std::unordered_map<std::wstring, Location> & map = assigned.top();
        const std::unordered_set<std::wstring> & out = context.getFunOut();
        for (const auto & n : out)
        {
            map.erase(n);
        }
        map.erase(fd.getSymbol().getName());
        for (const auto & p : map)
        {
            result.report(context, p.second, L"Declared variable and might be unused: %s.", p.first);
        }
        assigned.pop();
        used.pop();
    }
}

const std::wstring VariablesChecker::getName() const
{
    return L"VariablesChecker";
}
}
