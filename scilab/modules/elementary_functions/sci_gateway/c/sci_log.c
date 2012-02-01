/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA  - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
#include "../../core/src/c/parse.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "api_oldstack.h"
#include "msgs.h"

/*--------------------------------------------------------------------------*/
int sci_log(char *fname,void* pvApiCtx)
{
	SciErr sciErr;
	int i;
	int iAlert					= 1; // to print only one warning message
	int iRows						= 0;
	int iCols						= 0;
	int iType						= 0;

	int* piAddr					= NULL;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double* pdblRealRet	= NULL;
	double* pdblImgRet	= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddr, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(iType != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(isVarComplex(pvApiCtx, piAddr))
	{
		sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			if(iAlert && pdblReal[i] == 0 && pdblImg[i] == 0)
			{
				if(C2F(errgst).ieee == 0)
				{
					SciError(32);
					return 0;
				}
				else if(C2F(errgst).ieee == 1)
				{
					Msgs(64,0);
					iAlert = 0;
				}
			}

			wlog(pdblReal[i], pdblImg[i], &pdblRealRet[i], &pdblImgRet[i]);
		}
	}
	else
	{
		int iLessZero	= 0;
		sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		for(i = 0; i < iRows * iCols ; i++)
		{
			if(pdblReal[i] < 0)
			{
				iLessZero = 1;
			}
			else if(iAlert && pdblReal[i] == 0)
			{
				if(C2F(errgst).ieee == 0)
				{
					SciError(32);
					return 0;
				}
				else if(C2F(errgst).ieee == 1)
				{
					Msgs(64,0);
					iAlert = 0;
				}
			}
		}

		if(iLessZero == 0)
		{//All values > 0
			sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			for(i = 0; i < iRows * iCols ; i++)
			{
				pdblRealRet[i] = dlogs(pdblReal[i]);
			}
		}
		else
		{
			sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			for(i = 0; i < iRows * iCols ; i++)
			{
				wlog(pdblReal[i], 0, &pdblRealRet[i], &pdblImgRet[i]);
			}
		}
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
