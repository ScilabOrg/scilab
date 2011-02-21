/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "fileio_gw.hxx"
#include "function.hxx"
#include "arrayof.hxx"

extern "C"
{
#include <string.h>
#include "getshortpathname.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_getshortpathname(typed_list &in, int _iRetCount, typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d expected.\n"), L"getshortpathname" , 1);
        return Function::Error;
    }

    if(_iRetCount != 1 && _iRetCount != 2)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"getshortpathname", 1, 2);
        return Function::Error;
    }

    if(in[0]->isString() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Matrix of strings expected.\n"), L"getshortpathname", 1);
        return Function::Error;
    }

    String* pS  = in[0]->getAs<types::String>();

    String* pOut1 = new String(pS->getRows() , pS->getCols());
    Bool* pOut2 = new Bool(pS->getRows() , pS->getCols());
    int* pBool = pOut2->get();
    for(int i = 0 ; i < pS->getSize(); i++)
    {
        pOut1->set(i, getshortpathnameW(pS->get(i), &pBool[i]));
    }

    out.push_back(pOut1);
    if(_iRetCount == 2)
    {
        out.push_back(pOut2);
    }
    else
    {
        delete pOut2;
    }

    return Function::OK;
	//CheckRhs(0,1);
	//CheckLhs(1,2);

	//if (GetType(1) == sci_strings)
	//{
	//	int n1 = 0,m1 = 0, m1n1 = 0;
	//	char **LongNames = NULL;
	//	char **ShortNames = NULL;
	//	BOOL *bOK = NULL;
	//	int i = 0;

	//	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&LongNames);
	//	m1n1 = m1*n1;

	//	if (m1n1 > 0)
	//	{
	//		ShortNames = (char**) MALLOC(sizeof(char*)* (m1n1));
	//		bOK = (BOOL*) MALLOC(sizeof(BOOL)*(m1n1));

	//		if ( (ShortNames == NULL) || (bOK == NULL) )
	//		{
	//			freeArrayOfString(LongNames, m1n1);
	//			Scierror(999,"%s: Memory allocation error.\n", fname);
	//			return 0;
	//		}
	//	}

	//	for (i = 0;i < m1n1; i++)
	//	{
	//		ShortNames[i] = getshortpathname(LongNames[i], &bOK[i]);
	//	}
	//	freeArrayOfString(LongNames, m1n1);

	//	CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,ShortNames);
	//	LhsVar(1) = Rhs+1;

	//	if (Lhs == 2)
	//	{
	//		CreateVarFromPtr(Rhs+2,MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1, &bOK);
	//		LhsVar(2) = Rhs + 2;
	//	}

	//	C2F(putlhsvar)();

	//	freeArrayOfString(ShortNames, m1n1);
	//	if (bOK) {FREE(bOK); bOK = NULL;}
	//}
	//else
	//{
	//	Scierror(999,_("%s: Wrong type for input argument: A string expected.\n"),fname);
	//}
	//return 0;
}
/*--------------------------------------------------------------------------*/
