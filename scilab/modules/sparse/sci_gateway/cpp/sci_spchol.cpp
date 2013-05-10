/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
#include "Scierror.h"
#include "localization.h"
#include "sci_warning.h"
#include "sciprint.h"
}

types::Function::ReturnValue sci_spchol(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Sparse* pSpIn       = NULL;
    types::Sparse* pSpPermut   = NULL;
    types::Sparse* pSpFact     = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    //    if (_iRetCount != 2)
    //    {
    //        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "spchol", 2);
    //        return types::Function::Error;
    //    }

    if (in[0]->isSparse() == false && in[0]->isSparseBool() == false)
    {
        Scierror(999, _("%s: Wrong type for argument %d: Sparse matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    pSpIn = in[0]->getAs<types::Sparse>();

    if (pSpIn->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument %d: Real sparse matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    if (pSpIn->getRows() != pSpIn->getCols())
    {
        Scierror(999, _("%s: Wrong size for argument %d: Square sparse matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    int ierr = pSpIn->newCholLLT(pSpPermut, pSpFact);

    out.push_back(pSpFact);
    out.push_back(pSpPermut);

    return types::Function::OK;
}
