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

namespace jit
{

void JITVisitor::visit(const ast::DollarVar & e)
{
    if (const analysis::DollarInfo * di = e.getDecorator().getDollarInfo())
    {
        const ast::SimpleVar & var = di->getVar();
        const uint32_t index = di->getIndex();
        if (index == 1 || index == 2)
        {
            JITScilabPtr & x = getVariable(var.getSymbol(), var);
            if (index == 1)
            {
                if (di->getArgsCount() == 1)
                {
                    llvm::Value * rc = builder.CreateMul(x->loadRows(*this), x->loadCols(*this));
                    setResult(JITScilabPtr(new JITScalInt64(*this, rc, false, "")));
                }
                else
                {
                    setResult(JITScilabPtr(new JITScalInt64(*this, x->loadRows(*this), false, "")));
                }
            }
            else
            {
                setResult(JITScilabPtr(new JITScalInt64(*this, x->loadCols(*this), false, "")));
            }
        }
    }
}

}
