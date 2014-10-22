/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sparse_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_spchol(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Sparse *pSpOut = NULL;
    double abstol, reltol = 0.001;
    int *fmatindex = NULL, *nrank, *ierr = NULL;
    int m, n, sSize;
    types::Sparse *pSpIn = NULL;
    double *dbl = NULL, *imag = NULL;
    int *colPos = NULL;
    int *itemsRow = NULL;
    if (in.size() != 1 )
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 2 || _iRetCount < 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "spchol", 1, 2);
        return types::Function::Error;
    }

    if (!(in[0]->isSparse()) &&  !(in[0]->isSparseBool()))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Sparse matrix of scalars expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    pSpIn = in[0]->getAs<types::Sparse>();

    m = pSpIn->getRows();
    n = pSpIn->getCols();
    sSize = pSpIn->getSize();

    if (m != n)
    {
        Scierror(77, _("%s: Wrong size for input argument #%d: Square matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }
    if (pSpIn->isComplex())
    {
        Scierror(77, _("%s: Wrong type for argument %d: Real matrix expected.\n"), "lufact", 1);
        return types::Function::Error;
    }

    return types::Function::OK;
}
