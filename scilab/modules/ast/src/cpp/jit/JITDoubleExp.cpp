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

void JITVisitor::visit(const ast::DoubleExp & e)
{
    if (e.getDecorator().getResult().isAnInt())
    {
        setResult(JITScilabPtr(new JITScalInt64(*this, (int64_t)e.getValue(), false, "")));
    }
    else
    {
        if (types::Double * pDbl = static_cast<types::Double *>(e.getConstant()))
        {
            if (pDbl->isComplex())
            {
                if (pDbl->getSize() == 1)
                {
                    setResult(JITScilabPtr(new JITScalComplex(*this, std::complex<double>(pDbl->get(0), pDbl->getImg(0)), false, "")));
                }
                else
                {
                    llvm::Value * rows = getConstant<int64_t>(pDbl->getRows());
                    llvm::Value * cols = getConstant<int64_t>(pDbl->getCols());
                    llvm::Value * rc = getConstant<int64_t>(0);
                    llvm::Value * size = getConstant<int64_t>((int64_t)pDbl->getSize() * sizeof(double));
                    llvm::Value * alloc_re = callNew(size);
                    llvm::Value * alloc_im = callNew(size);
                    llvm::Value * dbl_alloc_re = builder.CreateBitCast(alloc_re, getTy<double *>());
                    llvm::Value * dbl_alloc_im = builder.CreateBitCast(alloc_im, getTy<double *>());
                    callMemcpy(alloc_re, getConstant<int8_t *>(reinterpret_cast<int8_t *>(pDbl->getReal())), getConstant<int32_t>(sizeof(double)), size);
                    callMemcpy(alloc_im, getConstant<int8_t *>(reinterpret_cast<int8_t *>(pDbl->getImg())), getConstant<int32_t>(sizeof(double)), size);
                    setResult(JITScilabPtr(new JITArrayofComplex(*this, dbl_alloc_re, dbl_alloc_im, rows, cols, rc, "")));
                }
            }
            else
            {
                if (pDbl->getSize() == 1)
                {
                    setResult(JITScilabPtr(new JITScalDouble(*this, pDbl->get(0), false, "")));
                }
                else
                {
                    llvm::Value * rows = getConstant<int64_t>(pDbl->getRows());
                    llvm::Value * cols = getConstant<int64_t>(pDbl->getCols());
                    llvm::Value * rc = getConstant<int64_t>(0);
                    llvm::Value * size = getConstant<int64_t>((int64_t)pDbl->getSize() * sizeof(double));
                    llvm::Value * alloc = callNew(size);
                    llvm::Value * dbl_alloc = builder.CreateBitCast(alloc, getTy<double *>());
                    callMemcpy(alloc, getConstant<int8_t *>(reinterpret_cast<int8_t *>(pDbl->get())), getConstant<int32_t>(sizeof(double)), size);
                    setResult(JITScilabPtr(new JITArrayofDouble(*this, dbl_alloc, rows, cols, rc, "")));
                }
            }
        }
        else
        {
            setResult(JITScilabPtr(new JITScalDouble(*this, e.getValue(), false, "")));
        }
    }
}

}
