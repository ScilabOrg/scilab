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
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "splitpath.h"
#include "localization.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_fileext(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "fileext" , 1);
        return Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "fileext", 1);
        return Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "fileext", 1);
        return Function::Error;
    }

    String* pS = in[0]->getAs<types::String>();
    String* pOut = new String(pS->getRows(), pS->getCols());


    for (int i = 0 ; i < pS->getSize() ; i++)
    {
        wchar_t* pwstIn         = pS->get(i);
        wchar_t* pwstDrive      = new wchar_t[wcslen(pwstIn) + 1];
        wchar_t* pwstDirectory  = new wchar_t[wcslen(pwstIn) + 1];
        wchar_t* pwstName       = new wchar_t[wcslen(pwstIn) + 1];
        wchar_t* pwstExtension  = new wchar_t[wcslen(pwstIn) + 1];

        splitpathW(pwstIn, FALSE, pwstDrive, pwstDirectory, pwstName, pwstExtension);
        pOut->set(i, pwstExtension);

        delete[] pwstDirectory;
        delete[] pwstDrive;
        delete[] pwstExtension;
        delete[] pwstName;
    }

    out.push_back(pOut);
    return Function::OK;
    //Rhs = std::max(Rhs,0);

    //CheckRhs(1,1);
    //CheckLhs(1,1);

    //if (GetType(1) == sci_strings)
    //{
    //	int n1 = 0, m1 = 0, l1 = 0;
    //	int i = 0;

    //	char **Input_filenames  = NULL;
    //	char **Output_extensions = NULL;

    //	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_filenames);

    //	Output_extensions = (char**)MALLOC(sizeof(char*)*(m1*n1));
    //	if (Output_extensions)
    //	{
    //
    //		for (i = 0; i < m1*n1; i++)
    //		{
    //			if (Input_filenames[i])
    //			{
    //				/* Bug 3089 */
    //				Output_extensions[i] = FindFileExtension(Input_filenames[i]);
    //			}
    //			else
    //			{
    //				Output_extensions[i] = NULL;
    //			}

    //			if (Output_extensions[i] == NULL)
    //			{
    //				Output_extensions[i] = os_strdup("");
    //			}
    //		}
    //		CreateVarFromPtr( Rhs+1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, Output_extensions );
    //		LhsVar(1)=Rhs+1;
    //		C2F(putlhsvar)();
    //		freeArrayOfString(Input_filenames, m1*n1);
    //		freeArrayOfString(Output_extensions,m1*n1);
    //	}
    //	else
    //	{
    //		freeArrayOfString(Input_filenames, m1*n1);
    //		Scierror(999,_("%s: No more memory.\n"),fname);
    //	}
    //}
    //else
    //{
    //	Scierror(999,_("%s: Wrong type for input argument: string expected.\n"),fname);
    //}
    //return 0;
}
/*--------------------------------------------------------------------------*/

