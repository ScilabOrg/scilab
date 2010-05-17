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
#ifndef _MSC_VER
#include <errno.h>
#endif 
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "FileExist.h"
#include "movefile.h"
#include "isdir.h"
#include "splitpath.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
static wchar_t* getFilenameWithExtensionForMove(wchar_t* wcFullFilename);
static void returnMoveFileResultOnStack(int ierr, char *fname, int* _piKey);
/*--------------------------------------------------------------------------*/
int sci_movefile(char *fname, int* _piKey)
{
	SciErr sciErr;
	int *piAddressVarOne = NULL;
	wchar_t *pStVarOne = NULL;
	int iType1 = 0;
	int lenStVarOne = 0;
	int m1 = 0, n1 = 0;

	int *piAddressVarTwo = NULL;
	wchar_t *pStVarTwo = NULL;
	int iType2 = 0;
	int lenStVarTwo = 0;
	int m2 = 0, n2 = 0;

	/* Check Input & Output parameters */
	CheckRhs(2,2);
	CheckLhs(1,2);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(_piKey, piAddressVarOne, &iType1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if (iType1 != sci_strings)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
		return 0;
	}

	sciErr = getVarDimension(_piKey, piAddressVarOne, &m1, &n1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if ( (m1 != n1) && (n1 != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
		return 0;
	}

	// get lenStVarOne
	sciErr = getMatrixOfWideString(_piKey, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	// get value of first argument only now (+ 2 if we add file separator @ the end)
	pStVarOne = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarOne + 2));
	if (pStVarOne == NULL)
	{
		Scierror(999,_("%s: Memory allocation error.\n"),fname);
		return 0;
	}
	
	sciErr = getMatrixOfWideString(_piKey, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarAddressFromPosition(_piKey, 2, &piAddressVarTwo);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(_piKey, piAddressVarTwo, &iType2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if (iType2 != sci_strings)
	{
		if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
		return 0;
	}

	sciErr = getVarDimension(_piKey, piAddressVarTwo, &m2, &n2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if ( (m2 != n2) && (n2 != 1) ) 
	{
		if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
		return 0;
	}

	sciErr = getMatrixOfWideString(_piKey, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	// get value of first argument only now (+ 2 if we add file separator @ the end)
	pStVarTwo = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarTwo + 2));
	if (pStVarTwo == NULL)
	{
		if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
		Scierror(999,_("%s: Memory allocation error.\n"),fname);
		return 0;
	}
	
	sciErr = getMatrixOfWideString(_piKey, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if ( isdirW(pStVarOne) || FileExistW(pStVarOne) )
	{
		int ierrMove = 0;

		if ( isdirW(pStVarOne)  )
		{
			/* move a directory into a directory */
			ierrMove = MoveDirectoryFunction(pStVarTwo, pStVarOne);
		}
		else if (FileExistW(pStVarOne))
		{
			if (isdirW(pStVarTwo)) 
			{
				/* move file into a existing directory */
				/* copy file into a existing directory */
				wchar_t* filename = getFilenameWithExtensionForMove(pStVarOne);
				if (filename)
				{
					#define FORMAT_FULLFILENAME "%s/%s"
					wchar_t* destFullFilename = NULL;

					/* remove last file separator if it exists */
					if ( (pStVarTwo[wcslen(pStVarTwo) - 1] == L'\\') || 
						(pStVarTwo[wcslen(pStVarTwo) - 1] == L'/') )
					{
						pStVarTwo[wcslen(pStVarTwo) - 1] = L'\0';
					}

					destFullFilename = (wchar_t*)MALLOC(sizeof(wchar_t)* ((int)wcslen(pStVarTwo) +
						(int)wcslen(filename) + (int)wcslen(L"/") + 1));
					wcscpy(destFullFilename, pStVarTwo);
					wcscat(destFullFilename, L"/");
					wcscat(destFullFilename, filename);

					ierrMove = MoveFileFunction(destFullFilename, pStVarOne);

					FREE(filename); filename = NULL;
					FREE(destFullFilename); destFullFilename = NULL;
				}
				else
				{
					if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
					if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}

					Scierror(999,_("%s: Memory allocation error.\n"),fname);
					return 0;
				}
			}
			else
			{
				/* move a file into a file */
				ierrMove = MoveFileFunction(pStVarTwo, pStVarOne);
			}
		}
		else
		{
			if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
			if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}
			Scierror(999,_("%s: Wrong value for input argument #%d: A valid filename or directory expected.\n"), fname, 1);
			return 0;
		}

		returnMoveFileResultOnStack(ierrMove, fname, _piKey);
	}
	else
	{
		Scierror(999,_("%s: Wrong value for input argument #%d: A valid filename or directory expected.\n"), fname, 1);
	}

	if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
	if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
static wchar_t* getFilenameWithExtensionForMove(wchar_t* wcFullFilename)
{
	wchar_t* wcfilename = NULL;

	if (wcFullFilename)
	{
		wchar_t *wcdrv = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));
		wchar_t* wcdir = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));
		wchar_t* wcname = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));
		wchar_t* wcext = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));
		wcfilename = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));

		splitpathW(wcFullFilename, FALSE, wcdrv, wcdir, wcname, wcext);

		wcscpy(wcfilename, wcname);
		wcscat(wcfilename, wcext);

		if (wcdrv) {FREE(wcdrv); wcdrv = NULL;}
		if (wcdir) {FREE(wcdir); wcdir = NULL;}
		if (wcname) {FREE(wcname); wcname = NULL;}
		if (wcext) {FREE(wcext); wcext = NULL;}
	}
	return wcfilename;
}
/*--------------------------------------------------------------------------*/
static void returnMoveFileResultOnStack(int ierr, char *fname, int* _piKey)
{
	double dError = 0.;
	wchar_t **sciError = NULL;
	int m_out = 1, n_out = 1;

	sciError = (wchar_t**)MALLOC(sizeof(wchar_t*)* 1);
	if (sciError == NULL)
	{
		Scierror(999,_("%s: Memory allocation error.\n"),fname);
		return;
	}

#ifdef _MSC_VER
	if (ierr)
	{
		#define BUFFER_SIZE 1024
		DWORD dw = GetLastError(); 
		wchar_t buffer[BUFFER_SIZE];

		if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
			dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),buffer, BUFFER_SIZE, NULL) == 0) 
		{
			wcscpy(buffer, L"Unknown Error");
		}

		// for compatibilty with copyfile, we return 0 (error)
		//dError = (double) dw;
		dError = (double) 0;

		sciError[0] = (wchar_t*)MALLOC(sizeof(wchar_t)* ((int)wcslen(buffer) + 1));
		if (sciError[0] == NULL)
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			return;
		}

		wcscpy(sciError[0], buffer);
	}
	else
	{
		dError = 1.;
		sciError[0] = (wchar_t*)MALLOC(sizeof(wchar_t)* 1);
		if (sciError[0] == NULL)
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			return;
		}
		wcscpy(sciError[0], L"");
	}
#else
	if (ierr)
	{
		// for compatibilty with copyfile, we return 0 (error)
		//dError = (double) ierr;
		dError = (double) 0.;

		sciError[0] = to_wide_string(strerror(errno));
		if (sciError[0] == NULL)
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			return;
		}
	}
	else
	{
		dError = 1.;
		sciError[0] = (wchar_t*)MALLOC(sizeof(wchar_t)* 1);
		if (sciError[0] == NULL)
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			return;
		}
		wcscpy(sciError[0], L"");
	}
#endif

	createMatrixOfDouble(_piKey, Rhs + 1, m_out, n_out, &dError);
	LhsVar(1) = Rhs + 1;

	if (Lhs == 2)
	{
		createMatrixOfWideString(_piKey, Rhs + 2, m_out, n_out, sciError);
		LhsVar(2) = Rhs + 2;
	}

	freeArrayOfWideString(sciError, 1);

	C2F(putlhsvar)();
}
/*--------------------------------------------------------------------------*/

