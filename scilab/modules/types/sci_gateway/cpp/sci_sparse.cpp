/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

Function::ReturnValue sci_sparse(typed_list &in, int _piRetCount, typed_list &out)
{
    InternalType *pRetVal = NULL;

    if ( (in.size() < 1) || (in.size() > 3) )
    {
        return Function::Error;
    }
    
    for(int i = 0 ; i < in.size() ; i++)
    {
        if(in[i]->getType() != InternalType::RealDouble && in[i]->getType() != InternalType::RealSparse)
        {
            return Function::Error;
        }
    }

// Working on as we speak.
    if(in.size() == 1)
    {
	if (in[0]->getType() == InternalType::RealDouble) {
		Double *pD = in[0]->getAsDouble();
		pRetVal = new Sparse(&pD);
//unfinished 
	}
	else {
		pRetVal = (InternalType *)in[0]->getAsSparse();
	}
    }
    else if(in.size() == 2)
    {
//unfinished
    }
    else
    {// size is 3
//unfinished
    }

    if(pRetVal == NULL)
    {
        return Function::Error;
    }

    out.push_back(pRetVal);

    return Function::OK;
}
