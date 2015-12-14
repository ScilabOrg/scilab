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


#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "calls/JITLength.hxx"

namespace jit
{

bool JITLength::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    const ast::exps_t args = ce.getArgs();

    if (args.size() == 1)
    {
        const analysis::TIType & firstTy = args.front()->getDecorator().getResult().getType();

        if (firstTy.isscalar())
        {
            if (out.empty())
            {
                out.emplace_back(new JITScalInt64(jit.getConstant<int64_t>(1)));
            }
            else
            {
                // we have rc = length(a, "*"), so rc is an int64_t
                out.front()->storeData(jit, jit.getConstant<int64_t>(1));
            }
        }
        else
        {
            args.front()->accept(jit);
            JITScilabPtr & firstRes = jit.getResult();
            llvm::Value * r = firstRes->loadRows(jit);
            llvm::Value * c = firstRes->loadCols(jit);
            llvm::Value * rc = jit.getBuilder().CreateMul(r, c);

            if (out.empty())
            {
                out.emplace_back(new JITScalInt64(rc));
            }
            else
            {
                out.front()->storeData(jit, rc);
            }
        }
        return true;
    }

    return false;
}
}

