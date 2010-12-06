/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "alltypes.hxx"
#include "sparse.hxx"
#include "funcmanager.hxx"

extern "C" {
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

using namespace types;
#define CONST

Function::ReturnValue sci_full(typed_list &in, int nbRes, typed_list &out)
{
    types::Sparse *pRetVal = NULL;

    // per the scilab manual sparse will take upto 3 arguments but no less than one
    if ( in.size() != 1 )
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"full", 1);
        return Function::Error;
    }
    if( in[0]->getType() != InternalType::RealSparse && in[0]->getType() != InternalType::RealSparseBool)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: sparse matrix expected.\n"), L"full", 1);
        return Function::Error;
    }
    if(nbRes > 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d expected.\n"), L"full", 1);
    }

    types::Sparse CONST* sp(in[0]->getAsSparse());
    if (sp)
    {
        Double* res (new types::Double(sp->rows_get(), sp->cols_get(), sp->isComplex()));
        if (res)
        {
            sp->fill(*res);
            out.push_back(res);
        }
    }
    else
    {
        types::SparseBool CONST* spb(in[0]->getAsSparseBool());
        Bool* res (new types::Bool(spb->rows_get(), spb->cols_get()));
        if (res)
        {
            spb->fill(*res);
            out.push_back(res);
        }
    }
    return Function::OK;
}
