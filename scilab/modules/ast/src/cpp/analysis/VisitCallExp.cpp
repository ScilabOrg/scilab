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

#include "AnalysisVisitor.hxx"

namespace analysis
{

void AnalysisVisitor::visit(ast::CallExp & e)
{
    visit(e, 1);
}

void AnalysisVisitor::visit(ast::CallExp & e, const unsigned int lhs)
{
    // TODO: e.getName() is not always a simple var: foo(a)(b)
    if (e.getName().isSimpleVar())
    {
        const ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.getName());
        const symbol::Symbol & sym = var.getSymbol();
        const std::wstring & name = sym.getName();
        Info & info = getSymInfo(sym); // that put the sym in the current block !
        Result & res = e.getName().getDecorator().setResult(info.type);
        res.setConstant(info.getConstant());
        res.setRange(info.getRange());
        res.setMaxIndex(info.getMaxIndex());

        logger.log(L"CallExp", e.getLocation(), name);

        if (info.type.type == TIType::MACRO || info.type.type == TIType::MACROFILE || info.type.type == TIType::FUNCTION)
        {
            if (name == L"error")
            {
                getDM().getCurrent()->setReturn(true);
            }

            // Special analysis cases: size, zeros, ones, ...
            MapSymCall::iterator it = symscall.find(name);
            if (it != symscall.end())
            {
                if (getCM().checkGlobalConstant(sym) && it->second.get()->analyze(*this, lhs, e))
                {
                    return;
                }
            }

            visitArguments(name, lhs, info.type, e, e.getArgs());
        }
        else
        {
            analyzeIndices(info.type, e);
        }
    }
}

} // namespace analysis
