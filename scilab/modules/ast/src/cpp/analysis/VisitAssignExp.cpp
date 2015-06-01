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

    void AnalysisVisitor::visit(ast::AssignExp & e)
    {
        logger.log(L"AssignExp", e.getLocation());
        if (e.getLeftExp().isSimpleVar()) // A = ...
        {
            ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.getLeftExp());
            const symbol::Symbol & sym = var.getSymbol();

            if (e.getRightExp().isSimpleVar())
            {
                // We have A = B (so the data associated to b is shared with a)
                const symbol::Symbol & symR = static_cast<ast::SimpleVar &>(e.getRightExp()).getSymbol();
                dm.share(sym, symR, getSymInfo(symR).getType(), &e);
            }
            else
            {
                if (e.getRightExp().isCallExp()) // A = foo(...)
                {
                    // apply the ConstantVisitor
                    e.getRightExp().accept(cv);
                    if (e.getRightExp().isCallExp())
                    {
                        visit(static_cast<ast::CallExp &>(e.getRightExp()), /* LHS */ 1);
                    }
                    else
                    {
                        e.getRightExp().accept(*this);
                    }
                }
                else // A = 1 + 2
                {
                    cv.setLHS(1);
                    e.getRightExp().accept(cv);
                    e.getRightExp().accept(*this);
                }

                Result & RR = getResult();
                // Don't remove tmp... temp.remove(RR);
                var.getDecorator().res = RR;
                Info & info = dm.define(sym, RR.getType(), &e);
                info.getConstant() = RR.getConstant();
            }
        }
        else if (e.getLeftExp().isCallExp()) // A(12) = ...
        {
            ast::CallExp & ce = static_cast<ast::CallExp &>(e.getLeftExp());
            if (ce.getName().isSimpleVar())
            {
                const symbol::Symbol & symL = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol();
                e.getRightExp().accept(*this);
                Result RR = getResult();
                ce.getDecorator().res = RR;
                Info & info = dm.write(symL, RR.getType(), &e);
                if (analyzeIndices(info.type, ce))
		{
		    e.getDecorator().safeInsertion = (RR.getType() == getResult().getType());
		}
            }
        }
        else if (e.getLeftExp().isAssignListExp()) // [A, B] = ...
        {
            ast::AssignListExp & ale = static_cast<ast::AssignListExp &>(e.getLeftExp());
            if (e.getRightExp().isCallExp())
            {
                const ast::exps_t & exps = ale.getExps();
                visit(static_cast<ast::CallExp &>(e.getRightExp()), /* LHS */ exps.size());
                std::vector<Result>::iterator j = multipleLHS.begin();
                for (const auto exp : exps)
                {
                    // TODO: handle fields...
                    if (exp->isSimpleVar() && j != multipleLHS.end())
                    {
                        const ast::SimpleVar & var = *static_cast<ast::SimpleVar *>(exp);
                        const symbol::Symbol & sym = var.getSymbol();
                        Info & info = dm.define(sym, j->getType(), exp);
                        info.setConstant(j->getConstant());
                        ++j;
                    }
                }
            }
        }
    }
}
