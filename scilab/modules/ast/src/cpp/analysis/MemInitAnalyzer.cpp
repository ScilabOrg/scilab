/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AnalysisVisitor.hxx"
#include "analyzers/MemInitAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{
bool MemInitAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    const ast::exps_t args = e.getArgs();
    if (args.size() == 2)
    {
        ast::Exp * first = *args.begin();
        ast::Exp * second = *std::next(args.begin());

        first->accept(visitor);
        Result R1 = visitor.getResult();
        visitor.getDM().releaseTmp(R1.getTempId(), first);
        second->accept(visitor);
        Result & R2 = visitor.getResult();
        visitor.getDM().releaseTmp(R2.getTempId(), second);
        double val;
        SymbolicDimension rows, cols;
        bool empty = false;

        if (R1.getConstant().getDblValue(val))
        {
            const int nrows = tools::cast<int>(val);
            if (nrows <= 0)
            {
                empty = true;
            }
            else
            {
                rows = SymbolicDimension(visitor.getGVN(), nrows);
            }
        }
        else if (GVN::Value * gvnValue = R1.getConstant().getGVNValue())
        {
            rows.setValue(gvnValue);
            rows.setGVN(&visitor.getGVN());
        }
        else
        {
            return false;
        }

        if (!empty)
        {
            if (R2.getConstant().getDblValue(val))
            {
                const int ncols = tools::cast<int>(val);
                if (ncols <= 0)
                {
                    empty = true;
                }
                else
                {
                    cols = SymbolicDimension(visitor.getGVN(), ncols);
                }
            }
            else if (GVN::Value * gvnValue = R2.getConstant().getGVNValue())
            {
                cols.setValue(gvnValue);
                cols.setGVN(&visitor.getGVN());
            }
            else
            {
                return false;
            }
        }

        if (empty)
        {
            e.getDecorator().setResult(TIType(visitor.getGVN(), TIType::EMPTY));
        }
        else
        {
            bool res = visitor.getCM().check(ConstraintManager::POSITIVE, rows.getValue());
            if (res)
            {
                res = visitor.getCM().check(ConstraintManager::POSITIVE, cols.getValue());
                if (!res)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
            TIType resT(visitor.getGVN(), TIType::DOUBLE, rows, cols);
            e.getDecorator().setResult(Result(resT, visitor.getDM().getTmpId(resT, false)));
        }
        visitor.setResult(e.getDecorator().res);

        return true;
    }

    return false;
}
}
