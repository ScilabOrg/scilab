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

using namespace types;

Function::ReturnValue sci_spones(typed_list &in, int _piRetCount, typed_list &out)
{
    types::Sparse *pRetVal = NULL;

    // per the scilab manual sparse will take upto 3 arguments but no less than one
    if ( in.size() != 1 || in[0]->getType() != InternalType::RealSparse)
    {
        return Function::Error;
    }
    pRetVal = in[0]->getAsSparse()->newOnes();
    if(pRetVal == 0)
    {
        return Function::Error;
    }

    out.push_back(pRetVal);

    return Function::OK;
}
