/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "sparse_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "sparse.hxx"
#include "pointer.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
#include "lu.h"
}

types::Function::ReturnValue sci_lufact(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Sparse *pSpIn = NULL;

    double abstol   = 0;
    double reltol   = 0.001;
    int *fmatindex   = NULL;
    int nrank       = 0;
    int ierr        = 0;
    int m           = 0;
    int n           = 0;
    int nonZeros    = 0;

    double* dbl     = NULL;
    int* colPos     = NULL;
    int* itemsRow   = NULL;

    //check input parameters
    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "lufact", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): At most %d expected.\n"), "lufact", 2);
        return types::Function::Error;
    }

    abstol = C2F(dlamch) ("p", 1L);

    //call format
    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(77, _("%s: Wrong type for argument %d: Matrix expected.\n"), "lufact", 2);
            return types::Function::Error;
        }

        types::Double *pDblPrec = in[1]->getAs<types::Double>();

        if (pDblPrec->getSize() > 2)
        {
            Scierror(77, _("%s: Wrong number of dimensions for input argument #%d.\n"), "lufact", 2);
            return types::Function::Error;
        }

        if (pDblPrec->getSize() == 2)
        {
            reltol = pDblPrec->get(1);
        }

        abstol = pDblPrec->get(0);
    }

    if (in[0]->isSparse() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_lufact";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    pSpIn = in[0]->getAs<types::Sparse>();

    m = pSpIn->getRows();
    n = pSpIn->getCols();

    if (m != n)
    {
        Scierror(77, _("%s: Wrong size for input argument #%d: Square matrix expected.\n"), "lufact", 1);
        return types::Function::Error;
    }

    if (pSpIn->isComplex())
    {
        Scierror(77, _("%s: Wrong type for argument %d: Real matrix expected.\n"), "lufact", 1);
        return types::Function::Error;
    }

    nonZeros = pSpIn->nonZeros();
    dbl = new double[nonZeros];
    colPos = new int[nonZeros];
    itemsRow = new int[m];
    fmatindex = new int[1];

    pSpIn->outputValues(dbl, NULL);
    pSpIn->getNbItemByRow(itemsRow);
    pSpIn->getColPos(colPos);

    C2F(lufact1)(dbl, itemsRow, colPos, &m, &nonZeros, fmatindex, &abstol, &reltol, &nrank, &ierr);

    out.push_back(new types::Pointer((void*)fmatindex));
    if (_iRetCount == 2)
    {
        out.push_back(new types::Double((double)nrank));
    }

    delete[] dbl;
    delete[] colPos;
    delete[] itemsRow;

    return types::Function::OK;
}
