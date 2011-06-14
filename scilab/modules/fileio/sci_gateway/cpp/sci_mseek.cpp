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
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"

extern "C"
{
#include <stdio.h>
#include "localization.h"
#include "Scierror.h"
#include "mseek.h"
}
/*--------------------------------------------------------------------------*/
#if (defined(sun) && !defined(SYSV))
char *strerror (int errcode);
#endif
/*--------------------------------------------------------------------------*/
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif
/*--------------------------------------------------------------------------*/

Function::ReturnValue sci_mseek(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFile           = -1; //default file : last opened file
    int iRet            = 0;
    int iWhere          = 0;
    int iFlag           = 0;
    wchar_t* wcsFlag    = NULL;

    if(in.size() < 1 || in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"mseek", 1, 3);
        return types::Function::Error;
    }

    if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A Real expected.\n"), L"mseek", 1);
        return types::Function::Error;
    }

    iWhere = static_cast<int>(in[0]->getAs<types::Double>()->getReal()[0]);

    if(in.size() == 2)
    {
        if(in[1]->isDouble() && in[1]->getAs<types::Double>()->isScalar() && in[1]->getAs<types::Double>()->isComplex() == false)
        {
            iFile = static_cast<int>(in[1]->getAs<types::Double>()->getReal()[0]);
        }
        else if(in[1]->isString() && in[1]->getAs<types::String>()->isScalar())
        {
            wcsFlag = in[1]->getAs<types::String>()->get(0);
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A real or String expected.\n"), L"mseek", 2);
            return types::Function::Error;
        }
    }

    if(in.size() == 3)
    {
        if(in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false || in[1]->getAs<types::Double>()->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A real expected.\n"), L"mseek", 2);
            return types::Function::Error;
        }
        if(in[2]->isString() == false || in[2]->getAs<types::String>()->isScalar() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"mseek", 3);
            return types::Function::Error;
        }

        iFile = static_cast<int>(in[1]->getAs<types::Double>()->getReal()[0]);
        wcsFlag = in[2]->getAs<types::String>()->get(0);
    }

    if(wcsFlag != NULL)
    {
        if(wcsncmp(wcsFlag, L"set",3) == 0)
        {
            iFlag = SEEK_SET;
        }
        else if(wcsncmp(wcsFlag, L"cur",3) == 0)
        {
    		iFlag = SEEK_CUR;
        }
        else if(wcsncmp(wcsFlag, L"end",3) == 0)
        {
		    iFlag = SEEK_END;
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: '%ls', '%ls' or '%ls' expected.\n"),L"mseek",3,L"set",L"cur",L"end");
            return types::Function::Error;
        }
    }
    else
    {
        iFlag = SEEK_SET;
    }

    int iErr = mseek(iFile, iWhere, iFlag);

    out.push_back(new Bool(!iErr));
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
