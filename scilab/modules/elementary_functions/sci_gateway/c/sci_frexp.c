/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_frexp(char *fname, void* pvApiCtx)
{
	SciErr sciErr;
	int i;
	int iRows					= 0;
	int iCols					= 0;
	int iType					= 0;


	int* piAddr				= NULL;


	double *pdblReal	= NULL;
	double *pdblCoef	= NULL;
	double *pdblExp		= NULL;

	CheckRhs(1,1);
	CheckLhs(2,2);

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
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
		Scierror(999,_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 1);
        return 0;
	}

	sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblCoef);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 2, iRows, iCols, &pdblExp);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	for(i = 0 ; i < iRows * iCols; i++)
	{
		pdblCoef[i] = dfrexps(pdblReal[i], &pdblExp[i]);
	}

	LhsVar(1) = Rhs + 1;
	LhsVar(2) = Rhs + 2;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
