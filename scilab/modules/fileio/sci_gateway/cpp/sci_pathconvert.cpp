/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "pathconvert.h"
}

#define UNIX_TYPE       L"u"
#define WINDOWS_TYPE    L"w"
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_pathconvert(typed_list &in, int _iRetCount, typed_list &out)
{
	PathConvertType PType   = AUTO_STYLE;
    int iPathExpand         = 1;
    int iPathTrail          = 1;

    if(in.size() < 1 && in.size() > 4)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d to %d expected.\n"), L"pathconvert" , 1, 4);
        return Function::Error;
    }

    if(_iRetCount != 1)
    {
        ScierrorW(78,_W("%ls: Wrong number of output argument(s): %d expected.\n"), L"pathconvert", 1);
        return Function::Error;
    }

    //get type
    if(in.size() > 3)
    {
        if(in[3]->isString() == false || in[3]->getAs<types::String>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"pathconvert", 4);
            return Function::Error;
        }

        wchar_t* pwstType = in[3]->getAs<types::String>()->get(0);
        if(wcscmp(pwstType, WINDOWS_TYPE) == 0)
        {
            PType = WINDOWS_STYLE;
        }
        else if(wcscmp(pwstType, UNIX_TYPE) == 0)
        {
            PType = UNIX_STYLE;
        }
        else
        {
            ScierrorW(999,_W("%ls: Wrong value for input argument #%d: '%ls' or '%ls' expected.\n"), L"pathconvert", 4, UNIX_TYPE, WINDOWS_TYPE);
            return Function::Error;
        }
    }

    if(in.size() > 2)
    {
        if(in[2]->isBool() == false || in[2]->getAs<types::Bool>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A boolean expected.\n"), L"pathconvert", 3);
            return Function::Error;
        }

        iPathExpand = in[2]->getAs<types::Bool>()->get()[0];
    }

    if(in.size() > 1)
    {
        if(in[1]->isBool() == false || in[1]->getAs<types::Bool>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A boolean expected.\n"), L"pathconvert", 2);
            return Function::Error;
        }

        iPathTrail = in[1]->getAs<types::Bool>()->get()[0];
    }

    if(in[0]->isDouble() && in[0]->getAs<Double>()->isEmpty())
    {
        out.push_back(Double::Empty());
        return Function::OK;
    }

    if(in[0]->isString() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Matrix of strings expected.\n"), L"pathconvert", 1);
        return Function::Error;
    }

    String* pS      = in[0]->getAs<types::String>();
    String* pOut    = new String(pS->getRows(), pS->getCols());
    wchar_t** pStr  = pOut->get();


    for(int i = 0 ; i < pS->getSize() ; i++)
    {
        pStr[i] = pathconvertW(pS->get(i), iPathTrail, iPathExpand, PType);
    }

    out.push_back(pOut);
    return Function::OK;
	//SciErr sciErr;
	//int *piAddressVarOne = NULL;
	//wchar_t **pStVarOne = NULL;
	//int iType1 = 0;
	//int *lenStVarOne = NULL;
	//int m1 = 0, n1 = 0;

	//wchar_t **results = NULL;
	//int i = 0;

	//BOOL flagtrail = TRUE;
	//BOOL flagexpand = TRUE;

	//PathConvertType PType = AUTO_STYLE;

	///* Check Input & Output parameters */
	//CheckRhs(1,4);
	//CheckLhs(1,1);


	//if (Rhs > 3)
	//{
	//	int *piAddressVarFour = NULL;
	//	wchar_t *pStVarFour = NULL;
	//	int iType4 = 0;
	//	int lenStVarFour = 0;
	//	int m4 = 0, n4 = 0;

	//	sciErr = getVarAddressFromPosition(_piKey, 4, &piAddressVarFour);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getVarType(_piKey, piAddressVarFour, &iType4);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	if (iType4 != sci_strings)
	//	{
	//		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 4);
	//		return 0;
	//	}

	//	sciErr = getVarDimension(_piKey, piAddressVarFour, &m4, &n4);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	if ( (m4 != n4) && (n4 != 1) )
	//	{
	//		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 4);
	//		return 0;
	//	}

	//	sciErr = getMatrixOfWideString(_piKey, piAddressVarFour, &m4, &n4, &lenStVarFour, &pStVarFour);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	pStVarFour = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarFour + 1));
	//	if (pStVarFour == NULL)
	//	{
	//		Scierror(999,_("%s: Memory allocation error.\n"),fname);
	//		return 0;
	//	}

	//	sciErr = getMatrixOfWideString(_piKey, piAddressVarFour, &m4, &n4, &lenStVarFour, &pStVarFour);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}


	//	if ( (wcscmp(pStVarFour, L"w") == 0) || (wcscmp(pStVarFour, L"u") == 0) )
	//	{
	//		if (wcscmp(pStVarFour, L"w") == 0)
	//		{
	//			PType = WINDOWS_STYLE;
	//		}
	//		else
	//		{
	//			PType = UNIX_STYLE;
	//		}
	//		if (pStVarFour) {FREE(pStVarFour); pStVarFour = NULL;}
	//	}
	//	else
	//	{
	//		if (pStVarFour) {FREE(pStVarFour); pStVarFour = NULL;}
	//		Scierror(999,_("%s: Wrong value for input argument #%d: ''w'' or ''u'' string expected.\n"), fname, 4);
	//		return 0;
	//	}
	//}

	//if (Rhs > 2)
	//{
	//	int *piAddressVarThree = NULL;
	//	int *piData = NULL;
	//	int iType3	= 0;
	//	int m3 = 0, n3 = 0;

	//	sciErr = getVarAddressFromPosition(_piKey, 3, &piAddressVarThree);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getVarType(_piKey, piAddressVarThree, &iType3);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	if (iType3 != sci_boolean)
	//	{
	//		Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
	//		return 0;
	//	}

	//	sciErr = getVarDimension(_piKey, piAddressVarThree, &m3, &n3);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}


	//	if ( (m3 != n3) && (n3 != 1) )
	//	{
	//		Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 3);
	//		return 0;
	//	}

	//	sciErr = getMatrixOfBoolean(_piKey, piAddressVarThree, &m3, &n3,  &piData);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	flagexpand = piData[0];
	//}

	//if (Rhs > 1)
	//{
	//	int *piAddressVarTwo = NULL;
	//	int *piData = NULL;
	//	int iType2	= 0;
	//	int m2 = 0, n2 = 0;

	//	sciErr = getVarAddressFromPosition(_piKey, 2, &piAddressVarTwo);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getVarType(_piKey, piAddressVarTwo, &iType2);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	if (iType2 != sci_boolean)
	//	{
	//		Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
	//		return 0;
	//	}

	//	sciErr = getVarDimension(_piKey, piAddressVarTwo, &m2, &n2);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	if ( (m2 != n2) && (n2 != 1) )
	//	{
	//		Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 2);
	//		return 0;
	//	}

	//	sciErr = getMatrixOfBoolean(_piKey, piAddressVarTwo, &m2, &n2,  &piData);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	flagtrail = piData[0];
	//}

	//sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
	//if(sciErr.iErr)
	//{
	//	printError(&sciErr, 0);
	//	return 0;
	//}

	//sciErr = getVarType(_piKey, piAddressVarOne, &iType1);
	//if(sciErr.iErr)
	//{
	//	printError(&sciErr, 0);
	//	return 0;
	//}

	//if (iType1 == sci_matrix)
	//{
	//	sciErr = getVarDimension(_piKey, piAddressVarOne, &m1, &n1);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	if ( (m1 == n1) && (m1 == 0) )
	//	{
	//		sciErr = createMatrixOfDouble(_piKey, Rhs + 1, m1, n1, NULL);
	//		if(sciErr.iErr)
	//		{
	//			printError(&sciErr, 0);
	//			return 0;
	//		}

	//		LhsVar(1) = Rhs + 1;
	//		C2F(putlhsvar)();
	//	}
	//	else
	//	{
	//		Scierror(999,_("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
	//	}
	//}
	//else if (iType1 == sci_strings)
	//{
	//	sciErr = getVarDimension(_piKey, piAddressVarOne, &m1, &n1);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	lenStVarOne = (int*)MALLOC(sizeof(int) * (m1 * n1));
	//	if (lenStVarOne == NULL)
	//	{
	//		Scierror(999,_("%s: Memory allocation error.\n"),fname);
	//		return 0;
	//	}

	//	results = (wchar_t **)MALLOC(sizeof(wchar_t*) * (m1 * n1));

	//	if (results == NULL)
	//	{
	//		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
	//		freeArrayOfWideString(pStVarOne, m1 * n1);
	//		Scierror(999,_("%s: Memory allocation error.\n"),fname);
	//		return 0;
	//	}

	//	sciErr = getMatrixOfWideString(_piKey, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	pStVarOne = (wchar_t**)MALLOC(sizeof(wchar_t*) * (m1 * n1));
	//	if (pStVarOne == NULL)
	//	{
	//		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
	//		Scierror(999,_("%s: Memory allocation error.\n"),fname);
	//		return 0;
	//	}

	//	for( i = 0; i < m1 * n1; i++)
	//	{
	//		pStVarOne[i] = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarOne[i] + 1));
	//		if (pStVarOne[i] == NULL)
	//		{
	//			if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
	//			freeArrayOfWideString(pStVarOne, m1 * n1);
	//			Scierror(999,_("%s: Memory allocation error.\n"),fname);
	//			return 0;
	//		}
	//	}

	//	sciErr = getMatrixOfWideString(_piKey, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	for( i = 0; i < m1 * n1; i++)
	//	{
	//		results[i] = pathconvertW(pStVarOne[i], flagtrail, flagexpand, PType);
	//	}

	//	sciErr = createMatrixOfWideString(_piKey, Rhs + 1, m1, n1, results);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	LhsVar(1) = Rhs + 1;
	//	C2F(putlhsvar)();

	//	if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
	//	freeArrayOfWideString(results, m1 * n1);
	//	freeArrayOfWideString(pStVarOne, m1 * n1);
	//}
	//else
	//{
	//	Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
	//	return 0;
	//}
	//return 0;
}
/*--------------------------------------------------------------------------*/
