/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include <wchar.h>
#include "callxpad.h"
#include "gw_xpad.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"


#include "loadOnUseClassPath.h"
#include "BOOL.h"
}

static BOOL loadedDep = FALSE;

/*--------------------------------------------------------------------------*/
int sci_xpad(char *fname, int* _piKey)
{
	SciErr sciErr;
	CheckRhs(0,2);
	CheckLhs(0,1);

    if (!loadedDep)
	{
		loadOnUseClassPath("Xpad");
		loadedDep = TRUE;
	}

    if (Rhs == 0)
	{
		callXpadW(NULL, 0);
	}
	else
	{
		int m1 = 0, n1 = 0;
		int *piAddressVarOne = NULL;
		wchar_t **pStVarOne = NULL;
		int *lenStVarOne = NULL;
		int i = 0;
		int iType1 = 0;

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
			Scierror(999,_("%s: Wrong type for argument %d: String matrix expected.\n"),fname,1);
			return 0;
		}

		/* get dimensions */
		sciErr = getMatrixOfWideString(_piKey, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		lenStVarOne = (int*)MALLOC(sizeof(int)*(m1 * n1));
		if (lenStVarOne == NULL)
		{
			Scierror(999,_("%s: No more memory.\n"), fname);
			return 0;
		}

		/* get lengths */
		sciErr = getMatrixOfWideString(_piKey, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		pStVarOne = (wchar_t  **)MALLOC(sizeof(wchar_t *)*(m1*n1));
		if (pStVarOne == NULL)
		{
			Scierror(999,_("%s: No more memory.\n"), fname);
			return 0;
		}

		for(i = 0; i < m1 * n1; i++)
		{
			pStVarOne[i] = (wchar_t *)MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
		}

		/* get strings */
		sciErr = getMatrixOfWideString(_piKey, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if(Rhs == 2) //get line numbers
		{
			int* piAddressVarTwo = NULL;
			int m2 = 0, n2 = 0;
			double* pdblVarTwo = NULL;
			int iType2 = 0;

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

			if (iType2 != sci_matrix)
			{
				Scierror(999,_("%s: Wrong type for argument %d: Real matrix expected.\n"),fname,2);
				return 0;
			}

			if(isVarComplex(_piKey, piAddressVarTwo) == 1)
			{
				Scierror(999,_("%s: Wrong type for argument %d: Real matrix expected.\n"),fname,2);
				return 0;
			}

			sciErr = getMatrixOfDouble(_piKey, piAddressVarTwo, &m2, &n2, &pdblVarTwo);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			if(m2 * n2 != m1 * n1)
			{
				Scierror(999,_("%s: Wrong size for input arguments #%d and #%d: Same dimensions expected.\n"),fname,1,2);
				return 0;
			}

			callXpadWWithLineNumber(pStVarOne, pdblVarTwo, m1 * n1);
		}
		else
		{
			callXpadW(pStVarOne,m1 * n1);
		}

		freeArrayOfWideString(pStVarOne,m1 * n1);
	}

	LhsVar(1) = 0;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
