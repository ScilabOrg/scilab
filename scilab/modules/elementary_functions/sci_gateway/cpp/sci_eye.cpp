/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "funcmanager.hxx"
#include "context.hxx"

extern "C"
{
#include "basic_functions.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_eye(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRows = 0;
    int iCols = 0;

    if(in.size() > 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"eye", 0, 2);
        return Function::Error;
    }

    if(in.size() == 2)
    {
        if(in[0]->isDouble() == false || in[0]->getAs<Double>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A scalar expected.\n"), L"eye", 1);
            return Function::Error;
        }

        if(in[1]->isDouble() == false || in[1]->getAs<Double>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A scalar expected.\n"), L"eye", 2);
            return Function::Error;
        }

        iRows = in[0]->getAs<Double>()->getReal()[0];
        iCols = in[1]->getAs<Double>()->getReal()[0];
    }

    if(in.size() == 1)
    {
        if(in[0]->isGenericType() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Matrix expected.\n"), L"eye", 1);
            return Function::Error;
        }

        iRows = in[0]->getAsGenericType()->getRows();
        iCols = in[0]->getAsGenericType()->getCols();
    }

    if(in.size() == 0)
    {
        iRows = -1;
        iCols = -1;
    }

    Double* pOut = new Double(iRows, iCols);
    pOut->setZeros();
    for(int i = 0 ; i < Min(iRows, iCols) ; i++)
    {
        pOut->setReal(i,i, 1);
    }

    if(iRows == -1 && iCols == -1)
    {
        pOut->getReal()[0] = 1;
    }
    out.push_back(pOut);
    return Function::OK;
	//SciErr sciErr;
	//int iRows							= 0;
	//int iCols							= 0;
	//int iType 						= 0;

	//int* piAddr1					= NULL;
	//int* piAddr2					= NULL;

	//double *pdblRealRet		= NULL;

	//static int id[6];
	//C2F(inteye)(id);
	//return 0;
	//CheckRhs(0,2);
	//CheckLhs(0,1);


	//if(Rhs <= 0)
	//{
	//	iRows = -1;
	//	iCols = -1;

	//	//special case for operator :
	//	if(Top == 0)
	//	{
	//		Top += 1;
	//		sciErr = allocMatrixOfDouble(pvApiCtx, Top, -1, -1, &pdblRealRet);
	//		if(sciErr.iErr)
	//		{
	//			printError(&sciErr, 0);
	//			return 0;
	//		}

	//		pdblRealRet[0] = 1;
	//		LhsVar(1) = Top;
	//		PutLhsVar();
	//	}
	//	else
	//	{
	//		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, -1, -1, &pdblRealRet);
	//		if(sciErr.iErr)
	//		{
	//			printError(&sciErr, 0);
	//			return 0;
	//		}

	//		pdblRealRet[0] = 1;
	//		LhsVar(1) = Rhs + 1;
	//		PutLhsVar();
	//	}
	//	return 0;
	//}
	//else if(Rhs == 1)
	//{
	//	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getVarType(pvApiCtx, piAddr1, &iType);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	switch(iType)
	//	{
	//	case sci_matrix :
	//	case sci_poly :
	//	case sci_boolean :
	//	case sci_ints :
	//	case sci_handles :
	//	case sci_strings :
	//		sciErr = getVarDimension(pvApiCtx, piAddr1, &iRows, &iCols);
	//		if(sciErr.iErr)
	//		{
	//			printError(&sciErr, 0);
	//			return 0;
	//		}

	//		break;
	//	default :
	//		OverLoad(1);
	//		return 0;
	//	}

	//}
	//else if(Rhs == 2)
	//{
	//	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getDimFromVar(pvApiCtx, piAddr1, &iRows);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getDimFromVar(pvApiCtx, piAddr2, &iCols);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}
	//}

	//if(iRows == 0 || iCols == 0)
	//{
	//	iRows = 0;
	//	iCols = 0;
	//}
	//else
	//{
	//	iRows = (int)dabss(iRows);
	//	iCols = (int)dabss(iCols);
	//}

	//sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
	//if(sciErr.iErr)
	//{
	//	printError(&sciErr, 0);
	//	return 0;
	//}


	//if(iRows * iCols != 0)
	//{
	//	deyes(pdblRealRet, iRows, iCols);
	//}

	//LhsVar(1) = Rhs + 1;
	//PutLhsVar();
	//return 0;
}
/*--------------------------------------------------------------------------*/
