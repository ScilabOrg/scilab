/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "string.hxx"

extern "C"
{
#include "mputl.h"
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_mputstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFile               = -1; //default file : last opened file
    types::String* pString  = NULL;
    int iErr                = 1;
    
    if(in.size() < 1 || in.size() > 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"mputstr", 1, 2);
        return types::Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"mputstr", 1);
        return types::Function::Error;
    }

    pString = in[0]->getAs<types::String>();

    if(in.size() == 2)
    {
        if(in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false|| in[1]->getAs<types::Double>()->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A real expected.\n"), L"mputstr", 2);
            return types::Function::Error;
        }
        iFile = static_cast<int>(in[1]->getAs<types::Double>()->get(0));
    }

    switch (iFile)
    {
        case 5: // stdin
            ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mputstr", iFile);
            return types::Function::Error;
        default :
            iErr = mputl(iFile, pString->get(), 1);
    }

    out.push_back(new Bool(!iErr));

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
