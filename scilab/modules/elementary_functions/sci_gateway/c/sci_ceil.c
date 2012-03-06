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
#include "MALLOC.h"
#include "basic_functions.h"
#include "api_scilab.h"

SciErr ceil_double(void* pvApiCtx, int* _piAddress);
SciErr ceil_poly(void* pvApiCtx, int* _piAddress);
SciErr ceil_int(void* pvApiCtx, int* _piAddress);

/*--------------------------------------------------------------------------*/
int sci_ceil(char *fname, void* pvApiCtx)
{
	SciErr sciErr;
	int iType 		= 0;

	int* piAddr		= NULL;

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

	switch(iType)
	{
	case sci_matrix :
		sciErr = ceil_double(pvApiCtx, piAddr);
		break;
	case sci_poly :
		sciErr = ceil_poly(pvApiCtx, piAddr);
		break;
	case sci_ints :
		sciErr = ceil_int(pvApiCtx, piAddr);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	PutLhsVar();
	return 0;
}

SciErr ceil_double(void* pvApiCtx, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows						= 0;
	int iCols						= 0;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet	= NULL;
	double *pdblImgRet	= NULL;

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iCols * iRows ; i++)
		{
			pdblRealRet[i] = dceils(pdblReal[i]);
			pdblImgRet[i] = dceils(pdblImg[i]);
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iCols * iRows ; i++)
		{
			pdblRealRet[i] = dceils(pdblReal[i]);
		}
	}

	LhsVar(1) = Rhs + 1;
	return sciErr;
}

SciErr ceil_poly(void* pvApiCtx, int* _piAddress)
{
	SciErr sciErr;
	int i,j;
	int iRows							= 0;
	int iCols							= 0;
	int iLen							= 0;
	double** pdblReal			= NULL;
	double** pdblImg			= NULL;
	double** pdblRealRet	= NULL;
	double** pdblImgRet		= NULL;
	int *piCoeff					= NULL;
	char* pstVarName			= NULL;

	sciErr = getPolyVariableName(pvApiCtx, _piAddress, pstVarName, &iLen);
	if(sciErr.iErr)
	{
		return sciErr;
	}
	
	pstVarName = (char*)MALLOC(sizeof(char) * (iLen + 1));

	sciErr = getPolyVariableName(pvApiCtx, _piAddress, pstVarName, &iLen);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)MALLOC(iRows * iCols * sizeof(int));
		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblReal		= (double**)MALLOC(sizeof(double*) * iRows * iCols);
		pdblImg			= (double**)MALLOC(sizeof(double*) * iRows * iCols);
		pdblRealRet	= (double**)MALLOC(sizeof(double*) * iRows * iCols);
		pdblImgRet	= (double**)MALLOC(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)MALLOC(sizeof(double) * piCoeff[i]);
			pdblImg[i]			= (double*)MALLOC(sizeof(double) * piCoeff[i]);
			pdblRealRet[i]	= (double*)MALLOC(sizeof(double) * piCoeff[i]);
			pdblImgRet[i]		= (double*)MALLOC(sizeof(double) * piCoeff[i]);
		}

		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; j < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j]	= dceils(pdblReal[i][j]);
				pdblImgRet[i][j]	= dceils(pdblImg[i][j]);
			}
		}

		sciErr = createComplexMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet, pdblImgRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		FREE(piCoeff);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			FREE(pdblReal[i]);
			FREE(pdblRealRet[i]);
		}
		FREE(pdblReal);
		FREE(pdblRealRet);

		if(isVarComplex(pvApiCtx, _piAddress))
		{
			for(i = 0 ; i < iRows * iCols ; i++)
			{
				FREE(pdblImg[i]);
				FREE(pdblImgRet[i]);
			}
			FREE(pdblImg);
			FREE(pdblImgRet);
		}
	}
	else
	{
		sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)MALLOC(iRows * iCols * sizeof(int));
		sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblReal		= (double**)MALLOC(sizeof(double*) * iRows * iCols);
		pdblRealRet	= (double**)MALLOC(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)MALLOC(sizeof(double) * piCoeff[i]);
			pdblRealRet[i]	= (double*)MALLOC(sizeof(double) * piCoeff[i]);
		}

		sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; j < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j]	= dceils(pdblReal[i][j]);
			}
		}

		sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		FREE(piCoeff);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			FREE(pdblReal[i]);
			FREE(pdblRealRet[i]);
		}
		FREE(pdblReal);
		FREE(pdblRealRet);
	}

	FREE(pstVarName);
	LhsVar(1) = Rhs + 1;
	return sciErr;
}

SciErr ceil_int(void* pvApiCtx, int* _piAddress)
{//No change for integer
	SciErr sciErr;sciErr.iErr = 0;
	LhsVar(1) = 1;
	return sciErr;
}
/*--------------------------------------------------------------------------*/
