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

    // extern void C2F(recbez)(double*, int*, double*, int*, double*, int*, double*, double*);
    // extern void C2F(dlamch) (const char *, unsigned long int);

    // extern void C2F(lufact1)(double*, int*, int*, int*, int*, int*, double*, double*, int*, int*);
}
types::Function::ReturnValue sci_lufact(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double  abstol, reltol = 0.001;
    int *fmatindex = NULL, *nrank, *ierr = NULL;
    int m, n, sSize;
    types::Sparse *pSpIn = NULL;
    double *dbl = NULL, *imag = NULL;
    int *colPos = NULL;
    int *itemsRow = NULL;

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
        if (!(in[1]->isDouble()))
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

    if (!(in[0]->isSparse()))
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_lufact";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }
    pSpIn = in[0]->getAs<types::Sparse>();

    m = pSpIn->getRows();
    n = pSpIn->getCols();
    sSize = pSpIn->getSize();

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

    //dbl = (wchar_t*)MALLOC(sizeof(wchar_t) * (_pI->getSize() + 1));
    dbl = (double*)MALLOC(sizeof(double) * sSize);
    colPos = (int*)MALLOC(sizeof(int) * m);
    itemsRow = (int*)MALLOC(sizeof(int) * n);

    ierr = (int*)MALLOC(sizeof(int));
    fmatindex = (int*)MALLOC(sizeof(int));
    nrank = (int*)MALLOC(sizeof(int));

    pSpIn->outputValues(dbl, NULL);

    C2F(lufact1)(dbl, pSpIn->getNbItemByRow(itemsRow) , pSpIn->getColPos(colPos) , &m , &sSize , fmatindex, &abstol, &reltol, nrank, ierr);

    out.push_back(new types::Pointer(fmatindex));

    if (_iRetCount == 2)
    {
        out.push_back(new types::Double((double)*nrank));
    }

    delete(dbl);
    delete(itemsRow);
    delete(ierr);
    delete(fmatindex);
    delete(nrank);

    return types::Function::OK;
}
