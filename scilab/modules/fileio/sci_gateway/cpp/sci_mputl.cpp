/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "yaspio.hxx"

extern "C"
{
#include "mputl.h"
#include "localization.h"
#include "Scierror.h"
#include "mopen.h"
#include "mclose.h"
#include "expandPathVariable.h"
}
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_mputl(typed_list &in, int _iRetCount, typed_list &out)
{
    int iFileID     = 0;
    int iErr        = 0;
    bool bCloseFile = false;

    if(in.size() != 2)
    {
        ScierrorW(999,_W("%ls: Wrong number of input argument(s): %d expected.\n"),L"mputl", 2);
        return Function::Error;
    }

    if(_iRetCount != 1)
    {
        ScierrorW(999,_W("%ls: Wrong number of output argument(s): %d expected.\n"), L"mputl", 1);
        return Function::Error;
    }

    if(in[1]->isDouble() && in[1]->getAs<Double>()->getSize() == 1)
    {
        iFileID = static_cast<int>(in[1]->getAs<Double>()->getReal()[0]);
    }
    else if(in[1]->isString() && in[1]->getAs<types::String>()->getSize() == 1)
    {
        wchar_t *expandedFileName = expandPathVariableW(in[1]->getAs<types::String>()->get(0));

        iErr = mopen(expandedFileName, L"wt", 0, &iFileID);
        FREE(expandedFileName);

        if(iErr)
        {
            switch(iErr)
            {
            case MOPEN_NO_MORE_LOGICAL_UNIT:
                ScierrorW(66, _W("%ls: Too many files opened!\n"), L"mputl");
                break;
            case MOPEN_CAN_NOT_OPEN_FILE:
                ScierrorW(999, _W("%ls: Cannot open file %ls.\n"), L"mputl", expandedFileName);
                break;
            case MOPEN_NO_MORE_MEMORY:
                ScierrorW(999, _W("%ls: No more memory.\n"), L"mputl");
                break;
            case MOPEN_INVALID_FILENAME:
                ScierrorW(999, _W("%ls: invalid filename %ls.\n"), L"mputl", expandedFileName);
                break;
            default: //MOPEN_INVALID_STATUS
                ScierrorW(999, _W("%ls: invalid status.\n"), L"mputl");
                break;
            }
            return Function::Error;
        }
        bCloseFile = true;
    }
    else
    {//Error
        Scierror(999,_("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), "mputl", 2);
        return Function::Error;
    }

    //String vextor, row or col
    if(in[0]->isString() == false || (in[0]->getAs<types::String>()->getRows() != 1 && in[0]->getAs<types::String>()->getCols() != 1))
    {
        ScierrorW(999,_W("%ls: Wrong size for input argument #%d: A 1-by-n or m-by-1 array expected.\n"), L"mputl", 1);
        return Function::Error;
    }

    String* pS = in[0]->getAs<types::String>();

	switch (iFileID)
	{
        case 5: // stdin
            ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mputl", iFileID);
            return types::Function::Error;
        default : iErr = mputl(iFileID, pS->get(), pS->getSize());
    }

    out.push_back(new Bool(!iErr));

    if(bCloseFile)
    {
        mclose(iFileID);
    }

    return Function::OK;


    //mputlErr = mputl(fileDescriptor, pStVarOne, mnOne);
    //freeArrayOfString(pStVarOne, mnOne);

    //if (bCloseFile)
    //{
    //    double dErrClose = 0.;
    //    C2F(mclose)(&fileDescriptor, &dErrClose);
    //    bCloseFile = FALSE;
    //}

    //switch (mputlErr)
    //{
    //case MPUTL_NO_ERROR:
    //    createScalarBoolean(pvApiCtx, Rhs + 1, TRUE);
    //    LhsVar(1) = Rhs + 1;
    //    PutLhsVar();
    //    break;

    //case MPUTL_INVALID_FILE_DESCRIPTOR:
    //    // commented for compatiblity
    //    // Scierror(999, _("%s: invalid file descriptor.\n"), fname);
    //    // break;
    //case MPUTL_ERROR:
    //case MPUTL_NO_WRITE_RIGHT:
    //default:
    //    createScalarBoolean(pvApiCtx, Rhs + 1, FALSE);
    //    LhsVar(1) = Rhs + 1;
    //    PutLhsVar();
    //    break;
    //}

    //return 0;
}
/*--------------------------------------------------------------------------*/
