/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include <string.h>
#include "sci_malloc.h"
#include "createdirectory.h"
#include "isdir.h"
#include "expandPathVariable.h"
#include "configvariable_interface.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
}
/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_createdir(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "createdir", 1);
        return Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "createdir", 1);
        return Function::Error;
    }

    wchar_t* pwstPath = expandPathVariableW(in[0]->getAs<types::String>()->get(0));
    int iRet = 0;
    if (!isdirW(pwstPath))
    {
        iRet = createdirectoryW(pwstPath);
    }
    else
    {
        if (getWarningMode())
        {
            sciprint(_("%ls: Warning: Directory '%ls' already exists.\n"), L"createdir", pwstPath);
        }
        iRet = 1;
    }

    FREE(pwstPath);
    Bool* pOut = new Bool(iRet);

    out.push_back(pOut);
    return Function::OK;
    //CheckRhs(1,1);
    //CheckLhs(0,1);

    //if (GetType(1) == sci_strings)
    //{
    //	BOOL bOK = FALSE;
    //	int m1 = 0, n1 = 0, l1 = 0;
    //	char *expandedpath = NULL;
    //	int out_n = 0;

    //	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

    //	expandedpath = expandPathVariable(cstk(l1));

    //	if (!isdir(expandedpath))
    //	{
    //		bOK = createdirectory(expandedpath);
    //	}
    //	else
    //	{
    //		if (getWarningMode()) sciprint(_("%s: Warning: Directory '%s' already exists.\n"),fname,expandedpath);
    //		bOK = TRUE;
    //	}

    //	if (expandedpath)
    //	{
    //		FREE(expandedpath);
    //		expandedpath = NULL;
    //	}

    //	m1 = 1; n1 = 1;
    //	CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1 ,&l1);
    //	*istk(l1) = bOK;

    //	LhsVar(1)=Rhs+1;
    //	C2F(putlhsvar)();
    //}
    //else
    //{
    //	Scierror(999,_("%s: Wrong type for input argument: string expected.\n"), fname);
    //}
    //return 0;
}
/*--------------------------------------------------------------------------*/
