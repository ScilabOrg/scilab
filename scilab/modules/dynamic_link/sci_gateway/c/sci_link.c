/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
#include "Scierror.h"
#include "dynamic_link.h"
#include "MALLOC.h"
#include "localization.h"
#include "dl_genErrorMessage.h"
#include "freeArrayOfString.h"
#include "os_strdup.h"
/*-----------------------------------------------------------------------------------*/
static int linkNoRhs(int *_piKey);
static int linkOneRhsShow(int *_piKey);
/*-----------------------------------------------------------------------------------*/
int sci_link(char *fname,int *_piKey)
{
    SciErr sciErr;
	BOOL fflag = FALSE;
	int idsharedlibrary = -1;

	char *SharedLibraryName = NULL;

	char **subname = NULL;
	int sizesubname = 0;
	int m2 = 0, n2 = 0;

	char *param3flag = NULL;

	int returnedID = -1;
	int ierr = 0;

    int* piAddress;
    int iType;

    int iRet = 0;

	CheckRhs(0,3);
	CheckLhs(1,1);

	if (Rhs == 0)
	{
		return linkNoRhs(_piKey);
	}
	else
	{
		if (Rhs >= 1)
		{
            sciErr = getVarAddressFromPosition(_piKey, 1, &piAddress);
            sciErr = getVarType(_piKey, piAddress, &iType);

			if (iType == sci_matrix)
			{
				int m1 = 0, n1 = 0;
                double* pdblReal = NULL;
                sciErr = getMatrixOfDouble(_piKey, piAddress, &m1, &n1, &pdblReal);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return sciErr.iErr;
				}
				if ( (m1 == n1) && (n1 == 1) )
				{
					idsharedlibrary = (int)pdblReal[0];
				}
				else
				{
					Scierror(999,_("%s : Wrong value for argument #%d: %s\n"),fname,1,_("Unique id of a shared library expected."));
					return 0;
				}
			}
			else if (iType == sci_strings)
			{
				char **strings = NULL;
				int m1 = 0, n1 = 0;
                iRet = getAllocatedMatrixOfString(_piKey, piAddress, &m1, &n1, &strings);
                if(iRet)
                {
                    freeAllocatedMatrixOfString(m1, n1, strings);
                    return iRet;
                }

				if ( (m1 == 1) && (n1 == 1) )
				{
					SharedLibraryName = os_strdup(strings[0]);
					freeArrayOfString(strings, m1*n1);
				}
				else
				{
                    freeAllocatedMatrixOfString(m1, n1, strings);
					Scierror(999,_("%s : Wrong type for input argument #%d: %s\n"),fname,1,_("Unique dynamic library name expected."));
					return 0;
				}

				if ( (Rhs == 1) && (strcmp(SharedLibraryName,"show")==0) )
				{
					return linkOneRhsShow(_piKey);
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input arguments: Strings expected.\n"),fname);
				return 0;
			}
		}

		if (Rhs >= 2)
		{
            sciErr = getVarAddressFromPosition(_piKey, 2, &piAddress);
            sciErr = getVarType(_piKey, piAddress, &iType);

			if (iType == sci_strings)
			{
                iRet = getAllocatedMatrixOfString(_piKey, piAddress, &m2, &n2, &subname);
                if(iRet)
                {
                    freeAllocatedMatrixOfString(m2, n2, subname);
                    return iRet;
                }
				if ( ((m2 == 1) && (n2 >= 1)) || ((m2 >= 1) && (n2 == 1)) )
				{
					if ((m2 == 1) && (n2 >= 1)) sizesubname = n2;
					if ((m2 >= 1) && (n2 == 1)) sizesubname = m2;
				}
				else
				{
					freeArrayOfString(subname,m2*n2);
					Scierror(999,_("%s: Wrong type for input argument. Strings vector expected.\n"),fname);
					return 0;
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument. Strings expected.\n"),fname);
				return 0;
			}
		}

		if (Rhs == 3)
		{
            sciErr = getVarAddressFromPosition(_piKey, 3, &piAddress);
			int iRows       = 0;
			int iCols       = 0;
			char** pstData  = NULL;

			iRet = getAllocatedMatrixOfString(_piKey, piAddress, &iRows, &iCols, &pstData);
			if(iRet)
			{
				freeAllocatedMatrixOfString(iRows, iCols, pstData);
				return iRet;
			}
			if ( ( strcmp(pstData, "f") == 0 ) || ( strcmp(pstData, "c") == 0 ) )
			{
				param3flag = os_strdup(pstData);
			}
			else
			{
				Scierror(999,_("%s Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname,3,"f","c");
				return 0;
			}
		}
		else
		{
			param3flag = os_strdup("f");
		}

		if (strcmp("f",param3flag)==0) fflag = TRUE;
		else fflag = FALSE;

		returnedID = scilabLink(idsharedlibrary,SharedLibraryName,subname,sizesubname,fflag,&ierr);
		if (ierr == 0)
		{
            iRet = createScalarInteger32(_piKey, Rhs + 1, returnedID);
            if(iRet)
            {
                return iRet;
            }
			LhsVar(1)=Rhs+1;
			PutLhsVar();
		}
		else
		{
			dl_genErrorMessage(fname, ierr, SharedLibraryName);
		}

		if (Rhs >= 2)
		{
			freeArrayOfString(subname,m2*n2);
		}

		if (SharedLibraryName) { FREE(SharedLibraryName); SharedLibraryName=NULL;}
	}

	if (param3flag) {FREE(param3flag); param3flag=NULL;}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int linkNoRhs(int* _piKey)
{
    SciErr sciErr;
    int iRet = 0;
	int sizeFunctionsList = 0;
	char ** FunctionsList = NULL;

	FunctionsList = getNamesOfFunctionsInSharedLibraries(&sizeFunctionsList);

	if ( (FunctionsList) && (sizeFunctionsList > 0) )
	{
        sciErr = createMatrixOfString(_piKey, Rhs + 1, 1, sizeFunctionsList, FunctionsList);
        freeArrayOfString(FunctionsList,sizeFunctionsList);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
		LhsVar(1) = Rhs+1;
		PutLhsVar();
	}
	else
	{
        iRet = createEmptyMatrix(_piKey, Rhs + 1);
        if(iRet)
        {
            return iRet;
        }
		LhsVar(1) = Rhs+1;
		PutLhsVar();
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int linkOneRhsShow(int *_piKey)
{
    SciErr sciErr;
    int iRet = 0;
	int m1 = 0, n1 = 0, l1 = 0;
	int *IdsList = NULL;
	int sizeIds = 0;

	ShowDynLinks();
	IdsList = getAllIdSharedLib(&sizeIds);

	if ( (sizeIds>0) && (IdsList) )
	{
        sciErr = createMatrixOfInteger32(_piKey, Rhs + 1, 1, sizeIds, IdsList);
 		if (IdsList)
        {
            FREE(IdsList);
            IdsList=NULL;
        }
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
	}
	else
	{
        iRet = createEmptyMatrix(_piKey, Rhs + 1);
        if(iRet)
        {
            return iRet;
        }
	}
	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
