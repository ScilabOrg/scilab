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

#include "checkers/FunctionTestReturnChecker.hxx"
#include "checkers/FindSymVisitor.hxx"

namespace slint
{
void FunctionTestReturnChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    if (ce.getName().isSimpleVar())
    {
        const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
        auto i = funs.find(name);
        if (i != funs.end())
        {
            const std::vector<unsigned int> & positions = i->second;
            if (!positions.empty())
            {
                const ast::AssignListExp * ale = nullptr;
                const ast::AssignExp * ae = context.getAssignExp();
                if (ae)
                {
                    if (ae->getLeftExp().isAssignListExp())
                    {
                        ale = &static_cast<const ast::AssignListExp &>(ae->getLeftExp());
                    }

                    // syms will contain the symbols associated to returned error code
                    std::set<symbol::Symbol> syms;
                    if (ale)
                    {
                        const ast::exps_t & exps = ale->getExps();
                        for (const auto pos : positions)
                        {
                            if (pos > exps.size())
                            {
                                result.report(context, e.getLocation(), *this, _("Function %s requires an error checking just after call."), name);
                                return;
                            }
                            else
                            {
                                if (exps[pos - 1] && exps[pos - 1]->isSimpleVar())
                                {
                                    syms.emplace(static_cast<ast::SimpleVar *>(exps[pos - 1])->getSymbol());
                                }
                            }
                        }
                    }
                    else if (ae->getLeftExp().isSimpleVar())
                    {
                        if (positions.size() > 1 || positions.back() != 1)
                        {
                            result.report(context, e.getLocation(), *this, _("Function %s requires an error checking just after call."), name);
                            return;
                        }
                        syms.emplace(static_cast<ast::SimpleVar &>(ae->getLeftExp()).getSymbol());
                    }
                    if (const ast::Exp * next = context.getNextRelevantExp())
                    {
                        if (next->isIfExp())
                        {
                            const ast::IfExp * ie = static_cast<const ast::IfExp *>(next);
                            // We look for syms in the test: found syms are removed from syms
                            FindSymVisitor(syms, ie->getTest());
                            if (!syms.empty())
                            {
                                result.report(context, e.getLocation(), *this, _("Function %s requires an error checking just after call."), name);
                                return;
                            }
                        }
                    }
                }
                else
                {
                    result.report(context, e.getLocation(), *this, _("Function %s requires an error checking just after call."), name);
                    return;
                }
            }
        }
    }
}

void FunctionTestReturnChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string FunctionTestReturnChecker::getName() const
{
    return "FunctionTestReturnChecker";
}
}
