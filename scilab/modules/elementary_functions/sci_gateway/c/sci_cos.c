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
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
#include "api_scilab.h"

/*--------------------------------------------------------------------------*/
int sci_cos(char *fname, void* pvApiCtx)
{
	SciErr sciErr;
	int i;
	int iRows							= 0;
	int iCols							= 0;
	int iType							= 0;
	int* piAddr						= NULL;

	double *pdblReal			= NULL;
	double *pdblImg				= NULL;
	double *pdblRealRet		= NULL;
	double *pdblImgRet		= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}

	sciErr = getVarType(pvApiCtx, piAddr, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
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
			return 1;
		}

		sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 1;
		}

		for(i = 0 ; i < iCols * iRows ; i++)
		{
			zcoss(pdblReal[i], pdblImg[i], &pdblRealRet[i], &pdblImgRet[i]);
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 1;
		}

		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 1;
		}

		for(i = 0 ; i < iCols * iRows ; i++)
		{
			pdblRealRet[i] = dcoss(pdblReal[i]);
		}
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
