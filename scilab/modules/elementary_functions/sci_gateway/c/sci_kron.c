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
#include "Scierror.h"
#include "api_oldstack.h"
#include "matrix_kronecker.h"

int conv_real_input(double* _pdblData, int _iSize);
int conv_img_input(double* _pdblReal, double* _pdblImg, int _iSize);

/*--------------------------------------------------------------------------*/
extern int C2F(intkron) (int *id);
extern int C2F(dcopy)();
extern int C2F(dscal)();

/*--------------------------------------------------------------------------*/
int sci_kron(char *fname,int* _piKey)
{
	SciErr sciErr;
	int iRows1					= 0;
	int iCols1					= 0;
	double *pdblReal1		= NULL;
	double *pdblImg1		= NULL;
	int iComplex1				= 0;

	int iRows2					= 0;
	int iCols2					= 0;
	double *pdblReal2		= NULL;
	double *pdblImg2		= NULL;
	int iComplex2				= 0;

	int iRowsRet				= 0;
	int iColsRet				= 0;
	double* pdblRealRet	= NULL;
	double* pdblImgRet	= NULL;
	int iComplexRet			= 0;

	int* piAddr1				= NULL;
	int* piAddr2				= NULL;

	CheckRhs(2,2);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	iComplex1 = isVarComplex(_piKey, piAddr1);
	iComplex2 = isVarComplex(_piKey, piAddr2);
	iComplexRet = iComplex1 | iComplex2;

	/*get first parameter*/
	if(isVarComplex(_piKey, piAddr1))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, piAddr1, &iRows1, &iCols1, &pdblReal1, &pdblImg1);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(_piKey, piAddr1, &iRows1, &iCols1, &pdblReal1);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	/*get second parameter*/
	if(isVarComplex(_piKey, piAddr2))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, piAddr2, &iRows2, &iCols2, &pdblReal2, &pdblImg2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(_piKey, piAddr2, &iRows2, &iCols2, &pdblReal2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	if(iComplexRet)
	{
		iRowsRet = iRows1 * iRows2;
		iColsRet = iCols1 * iCols2;
		sciErr = allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRowsRet, iColsRet, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}
	else
	{
		iRowsRet = iRows1 * iRows2;
		iColsRet = iCols1 * iCols2;
		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRowsRet, iColsRet, &pdblRealRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}


	/*Specials cases ./. and .\. */
	if(Fin == 20) // operator is ./.
	{
		int iRet = 0;
		if(iComplex2)
		{
			iRet = conv_img_input(pdblReal2, pdblImg2, iRows2 * iCols2);
			if(iRet)
			{
				return 1;
			}
		}
		else
		{
			iRet = conv_real_input(pdblReal2, iRows2 * iCols2);
			if(iRet)
			{
				return 1;
			}
		}
	}
	else if(Fin == 21) // operator is .\.
	{
		int iRet = 0;
		if(iComplex1)
		{
			iRet = conv_img_input(pdblReal1, pdblImg1, iRows1 * iCols1);
			if(iRet)
			{
				return 1;
			}
		}
		else
		{
			iRet = conv_real_input(pdblReal1, iRows1 * iCols1);
			if(iRet)
			{
				return 1;
			}
		}
	}

	if(iComplex1 == 0 && iComplex2 == 0)
	{//A real and B real
		vKronR(	pdblReal1, iRows1, iRows1, iCols1,
						pdblReal2, iRows2, iRows2, iCols2,
						pdblRealRet, iRows1 * iRows2);
	}
	else if(iComplex1 == 1 && iComplex2 == 0)
	{//A complex and B real
		/*Real part*/
		vKronR(	pdblReal1, iRows1, iRows1, iCols1,
						pdblReal2, iRows2, iRows2, iCols2,
						pdblRealRet, iRows1 * iRows2);
		/*Img part*/
		vKronR(	pdblImg1, iRows1, iRows1, iCols1,
						pdblReal2, iRows2, iRows2, iCols2,
						pdblImgRet, iRows1 * iRows2);
	}
	else if(iComplex1 == 0 && iComplex2 == 1)
	{//A real and B complex
		/*Real part*/
		vKronR(	pdblReal1, iRows1, iRows1, iCols1,
						pdblReal2, iRows2, iRows2, iCols2,
						pdblRealRet, iRows1 * iRows2);
		/*Img part*/
		vKronR(	pdblReal1, iRows1, iRows1, iCols1,
						pdblImg2, iRows2, iRows2, iCols2,
						pdblImgRet, iRows1 * iRows2);
	}
	else
	{//A complex and B complex
		/*Real part*/
		vKronC(	pdblReal1, pdblImg1, iRows1, iRows1, iCols1,
						pdblReal2, pdblImg2, iRows2, iRows2, iCols2,
						pdblRealRet, pdblImgRet, iRows1 * iRows2);
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int conv_real_input(double* _pdblData, int _iSize)
{
	int i;
	for(i = 0 ; i < _iSize ; i++)
	{
		if(_pdblData[i] != 0)
		{
			_pdblData[i] = 1.0 / _pdblData[i];
		}
		else
		{
			SciError(27);
			return 1;
		}
	}
	return 0;
}

int conv_img_input(double* _pdblReal, double* _pdblImg, int _iSize)
{
	int i;
	for(i = 0 ; i < _iSize ; i++)
	{
		double dblR			= _pdblReal[i];
		double dblI			= _pdblImg[i];

		double dblTemp	= dblR * dblR + dblI + dblI;
		if(dblTemp != 0)
		{
			_pdblReal[i]	= _pdblReal[i]	/ dblTemp;
			_pdblImg[i]	= - _pdblImg[i] / dblTemp;
		}
		else
		{
			SciError(27);
			return 1;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
