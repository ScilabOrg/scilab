/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* ...
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
#include "file.hxx"

extern "C"
{
#include "mget.h"
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "os_strdup.h"
}
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_mget(typed_list &in, int _iRetCount, typed_list &out)
{
    char* pstType   = os_strdup("l");//default type value : long
    int iSize       = 0;
    int iFile       = -1; //default file : last opened file
    double* pData   = NULL;
    int iErr        = 0;

    if(in.size() < 1 || in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"mget", 1, 3);
        return Function::Error;
    }

    //check parameter 1
    if(in[0]->isDouble() == false || in[0]->getAs<Double>()->getSize() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer expected.\n"), L"mget", 1);
        return Function::Error;
    }

    iSize = static_cast<int>(in[0]->getAs<Double>()->getReal()[0]);

    if(in.size() >= 2)
    {//export format
        if(in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"mget", 2);
            return Function::Error;
        }
        pstType = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
    }

    if(in.size() == 3)
    {
        if(in[2]->isDouble() == false || in[2]->getAs<Double>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer expected.\n"), L"mget", 3);
            return Function::Error;
        }
        iFile = static_cast<int>(in[2]->getAs<Double>()->getReal()[0]);
    }

    Double* pD = new Double(1, iSize);
    pData = pD->getReal();

    switch (iFile)
    {
        case 0: // stderr
        case 6: // stdout
            ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mget", iFile);
            return types::Function::Error;
    }

    C2F(mget)(&iFile, pData, &iSize, pstType, &iErr);

    if(iErr > 0)
    {
        return Function::Error;
    }

    if(iErr < 0) //no error
    {
        int iNewSize = (-iErr) - 1;
        if(iNewSize < iSize)
        {//read data are smaller then excepted size
            Double* pNewD = new Double(1, iNewSize);
            double* pNewData = pNewD->getReal();
            for(int i = 0 ; i < iNewSize ; i++)
            {
                pNewData[i] = pData[i];
            }

            delete pD;
            pD = pNewD;
        }
    }
    out.push_back(pD);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
