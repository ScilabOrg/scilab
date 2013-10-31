/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "optimization_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_readmps(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double dLowBnd = 0;
    double dUpBnd  = 0;
    double dMaxM   = 0;
    double dMaxN   = 0;
    double dMaxNza = 0;
    int iMode[2]   = { -1, 0};
    wchar_t* wcsFileName = NULL;


    if (in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "readmps", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "readmps", 1);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get file name
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "readmps", 1);
        return types::Function::Error;
    }

    types::String* pStrFileName = in[0]->getAs<types::String>();

    if (pStrFileName->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar string expected.\n"), "readmps", 1);
        return types::Function::Error;
    }

    wcsFileName = expandPathVariableW(pStrFileName->get(0));
    iErr = mopen(pwstTemp, pstMode, iSwap, &iID);

    if (FileManager::getFile(iFile) == NULL)
    {
        Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mfprintf", iFile);
        return types::Function::Error;
    }

    // get Bounds
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "readmps", 2);
        return types::Function::Error;
    }

    types::Double* pDblBounds = in[1]->getAs<types::Double>();

    if (pDblBounds->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "readmps", 2);
        return types::Function::Error;
    }

    if (pDblBounds->getSize() != 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real vector of size %d expected.\n"), "readmps", 2, 2);
        return types::Function::Error;
    }

    dLowBnd = pDblBounds->get(0);
    dUpBnd  = pDblBounds->get(1);

    if (_iRetCount == 3)
    {
        // get Max sizes
        if (in[2]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "readmps", 3);
            return types::Function::Error;
        }

        types::Double* pDblMaxSizes = in[2]->getAs<types::Double>();

        if (pDblMaxSizes->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "readmps", 3);
            return types::Function::Error;
        }

        if (pDblMaxSizes->getSize() != 3)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real vector of size %d expected.\n"), "readmps", 3, 3);
            return types::Function::Error;
        }

        dMaxM   = pDblMaxSizes->get(0);
        dMaxN   = pDblMaxSizes->get(1);
        dMaxNza = pDblMaxSizes->get(2);
    }
    else
    {

        C2F(rdmpsz)(lunit, m, n, nza, ierr, typrow, line);
    }

    /*** read MPS file ***/

    /*** return output arguments ***/

    return types::Function::OK;
}

