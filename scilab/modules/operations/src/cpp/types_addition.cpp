/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "types_addition.hxx"
#include "core_math.h"

#include "scilabexception.hxx"

extern "C"
{
    #include <stdio.h>
	#include "matrix_addition.h"
    #include "localization.h"
    #include "charEncoding.h"
    #include "os_swprintf.h"
}

InternalType *GenericPlus(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE + DOUBLE
    */
    if(TypeR == GenericType::RealDouble && TypeL == GenericType::RealDouble)
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = AddDoubleToDouble(pL, pR, (Double**)&pResult);
        if(iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, _W("Error: operator +: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

// FIXME: Overload or dedicated function.
//    else if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
//    {
//        //nothing to do, all in macro : %b_+_b
//    }

    /*
    ** STRING + STRING
    */
    else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
    {
        String *pL = _pLeftOperand->getAs<String>();
        String *pR = _pRightOperand->getAs<String>();

        int iResult = AddStringToString(pL, pR, (String**)&pResult);

        if(iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

// FIXME: Overload or dedicated function.
//    else if(TypeL == GenericType::RealInt && TypeR == GenericType::RealInt)
//    {
//    }

    /*
    ** DOUBLE + POLY
    */
    else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
    {
        Double *pL          = _pLeftOperand->getAs<Double>();
        MatrixPoly *pR      = _pRightOperand->getAs<MatrixPoly>();

        int iResult = AddDoubleToPoly(pR, pL, (MatrixPoly**)&pResult);
        if(iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    /*
    ** POLY + DOUBLE
    */
    else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
    {
        Double *pR	        = _pRightOperand->getAs<Double>();
        MatrixPoly *pL      = _pLeftOperand->getAs<MatrixPoly>();

        int iResult = AddDoubleToPoly(pL, pR, (MatrixPoly**)&pResult);
        if(iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    /*
    ** POLY + POLY
    */
    else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
    {
        MatrixPoly *pL	= _pLeftOperand->getAs<MatrixPoly>();
        MatrixPoly *pR	= _pRightOperand->getAs<MatrixPoly>();

        int iResult = AddPolyToPoly(pL, pR, (MatrixPoly**)&pResult);
        if(iResult != 0)
        {
            if(iResult == 1)
            {
                wchar_t pMsg[bsiz];
                os_swprintf(pMsg, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
                throw ast::ScilabError(pMsg);
            }
            else if(iResult == 2)
            {
                std::wostringstream os;
                os << L"variables don't have the same formal variable";
                //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw ast::ScilabError(os.str());
            }
        }
        return pResult;
    }

    /*
    ** DOUBLE + STRING
    */
    else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealString)
    {
        if(_pLeftOperand->getAs<Double>()->size_get() == 0)
        {//[] + "" -> ""
            return _pRightOperand->clone();
        }
        else
        {
            // Don't know how to manage this Addition : Return NULL will Call Overloading.
            return NULL;
        }
    }

    /*
    ** STRING + DOUBLE
    */
    else if(TypeL == GenericType::RealString && TypeR == GenericType::RealDouble)
    {
        if(_pRightOperand->getAs<Double>()->size_get() == 0)
        {//"text" + [] -> ""
            return _pLeftOperand->clone();
        }
        else
        {
            // Don't know how to manage this Addition :  Return NULL will Call Overloading.
            return NULL;
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}


int AddDoubleToDouble(Double *_pDouble1, Double *_pDouble2, Double** _pDoubleOut)
{
	bool bComplex1 			= _pDouble1->isComplex();
	bool bComplex2 			= _pDouble2->isComplex();
	bool bScalar1				= _pDouble1->rows_get() == 1 && _pDouble1->cols_get() == 1;
	bool bScalar2				= _pDouble2->rows_get() == 1 && _pDouble2->cols_get() == 1;
	bool bIdentity1			= _pDouble1->isIdentity();
	bool bIdentity2			= _pDouble2->isIdentity();
	bool bEmpty1				= _pDouble1->rows_get() == 0 && _pDouble1->cols_get() == 0;
	bool bEmpty2				= _pDouble2->rows_get() == 0 && _pDouble2->cols_get() == 0;

	double *pReal			= NULL;
	double *pImg			= NULL;

	if(bEmpty1)
	{
		if(bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal);
			memcpy(pReal, _pDouble2->real_get(), sizeof(double) * _pDouble2->rows_get() * _pDouble2->cols_get());
		}
		else
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			memcpy(pReal, _pDouble2->real_get(), sizeof(double) * _pDouble2->rows_get() * _pDouble2->cols_get());
			memcpy(pImg, _pDouble2->img_get(), sizeof(double) * _pDouble2->rows_get() * _pDouble2->cols_get());
		}
	}
	else if(bEmpty2)
	{
		if(bComplex1 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal);
			memcpy(pReal, _pDouble1->real_get(), sizeof(double) * _pDouble1->rows_get() * _pDouble1->cols_get());
		}
		else
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			memcpy(pReal, _pDouble1->real_get(), sizeof(double) * _pDouble1->rows_get() * _pDouble1->cols_get());
			memcpy(pImg, _pDouble1->img_get(), sizeof(double) * _pDouble1->rows_get() * _pDouble1->cols_get());
		}
	}
	else if(bIdentity1)
	{
		if(bComplex1 == false && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal);
			iAddRealIdentityToRealMatrix(
					_pDouble1->real_get()[0],
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iAddRealIdentityToComplexMatrix(
					_pDouble1->real_get()[0],
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iAddComplexIdentityToRealMatrix(
					_pDouble1->real_get()[0], _pDouble1->img_get()[0],
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iAddComplexIdentityToComplexMatrix(
					_pDouble1->real_get()[0], _pDouble1->img_get()[0],
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
	}
	else if(bIdentity2)
	{
		if(bComplex1 == false && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal);
			iAddRealIdentityToRealMatrix(
					_pDouble2->real_get()[0],
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iAddComplexIdentityToRealMatrix(
					_pDouble2->real_get()[0], _pDouble2->img_get()[0],
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iAddRealIdentityToComplexMatrix(
					_pDouble2->real_get()[0],
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iAddComplexIdentityToComplexMatrix(
					_pDouble2->real_get()[0], _pDouble2->img_get()[0],
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
	}
	else if(bScalar1)
	{//add pL with each element of pR
		if(bComplex1 == false && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal);
			iAddRealScalarToRealMatrix(
					_pDouble1->real_get()[0],
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iAddRealScalarToComplexMatrix(
					_pDouble1->real_get()[0],
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iAddComplexScalarToRealMatrix(
					_pDouble1->real_get()[0], _pDouble1->img_get()[0],
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iAddComplexScalarToComplexMatrix(
					_pDouble1->real_get()[0], _pDouble1->img_get()[0],
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
	}
	else if(bScalar2)
	{//add pL with each element of pR
		if(bComplex1 == false && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal);
			iAddRealScalarToRealMatrix(
					_pDouble2->real_get()[0],
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iAddComplexScalarToRealMatrix(
					_pDouble2->real_get()[0], _pDouble2->img_get()[0],
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iAddRealScalarToComplexMatrix(
					_pDouble2->real_get()[0],
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iAddComplexScalarToComplexMatrix(
					_pDouble2->real_get()[0], _pDouble2->img_get()[0],
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
	}
	else if(_pDouble1->rows_get() == _pDouble2->rows_get() && _pDouble1->cols_get() == _pDouble2->cols_get())//same dimension
	{//add pL and pR element wise
		if(bComplex1 == false && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal);
			iAddRealMatrixToRealMatrix(
					_pDouble1->real_get(),
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iAddRealMatrixToComplexMatrix(
					_pDouble1->real_get(),
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iAddRealMatrixToComplexMatrix(
					_pDouble2->real_get(),
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			(*_pDoubleOut)	= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iAddComplexMatrixToComplexMatrix(
					_pDouble1->real_get(), _pDouble1->img_get(),
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
	}
	else
	{
		return 1;
	}

	return 0;
}

int AddDoubleToPoly(MatrixPoly *_pPoly, Double *_pDouble, MatrixPoly ** _pPolyOut)
{
	bool bComplex1 			= _pPoly->isComplex();
	bool bComplex2 			= _pDouble->isComplex();
	bool bScalar1				= _pPoly->rows_get() == 1 && _pPoly->cols_get() == 1;
	bool bScalar2				= _pDouble->rows_get() == 1 && _pDouble->cols_get() == 1;

	double *pInDblR			= _pDouble->real_get();
	double *pInDblI			= _pDouble->img_get();

	if(bScalar1)
	{//cas balaise
		int *piRank = new int[_pDouble->size_get()];
		for(int i = 0 ; i < _pDouble->size_get() ; i++)
		{
			piRank[i] = _pPoly->poly_get(0)->rank_get();
		}

		(*_pPolyOut) = new MatrixPoly(_pPoly->var_get(), _pDouble->rows_get(), _pDouble->cols_get(), piRank);
		if(_pDouble->isComplex() || _pPoly->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
		}

		for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(0);
			Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0] = pInDblR[i] + pInPolyR[0];

			for(int j = 1 ; j < pInPoly->rank_get() ; j++)
			{
				pOutPolyR[j] = pInPolyR[j];
			}
		}

		if((*_pPolyOut)->isComplex())
		{
			for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(0);
				Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) + (pInPolyI != NULL ? pInPolyI[0] : 0);

				for(int j = 1 ; j < pInPoly->rank_get() ; j++)
				{
					pOutPolyI[j] = (pInPolyI != NULL ? pInPolyI[j] : 0);
				}
			}
		}
	}
	else if(bScalar2)
	{
		//Create new Poly
		(*_pPolyOut) = new MatrixPoly();
		//Copy the old one ( rank, size, ... but not coefficients
		**_pPolyOut = *_pPoly;

		for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0]			= pInDblR[0] + pInPolyR[0];
		}

		if(bComplex1 || bComplex2)
		{
			(*_pPolyOut)->complex_set(true);
			for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0]			= (pInDblI == NULL ? 0 : pInDblI[0]) + (pInPolyI == NULL ? 0 : pInPolyI[0]);
			}
		}
	}
	else if (_pDouble->rows_get() == _pPoly->rows_get() && _pDouble->cols_get() == _pPoly->cols_get())
	{
		(*_pPolyOut) = new MatrixPoly();
		//Create new Poly
		**_pPolyOut = *_pPoly;

		for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0]			= pInDblR[i] + pInPolyR[0];
		}

		if(bComplex1 || bComplex2)
		{
			(*_pPolyOut)->complex_set(true);
			for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0]			= (pInDblI != NULL ? pInDblI[i] : 0) + (pInPolyI != NULL ? pInPolyI[0] : 0);
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

int AddPolyToPoly(MatrixPoly* _pPoly1, MatrixPoly* _pPoly2, MatrixPoly ** _pPolyOut)
{
	if(_pPoly1->var_get() != _pPoly2->var_get())
	{
		return 2;
	}

	bool bComplex1 			= _pPoly1->isComplex();
	bool bComplex2 			= _pPoly2->isComplex();
	bool bScalar1				= _pPoly1->rows_get() == 1 && _pPoly1->cols_get() == 1;
	bool bScalar2				= _pPoly2->rows_get() == 1 && _pPoly2->cols_get() == 1;
	//3 cases :
	//size(p1) == size(P2)
	if(_pPoly1->rows_get() == _pPoly2->rows_get() && _pPoly1->cols_get() == _pPoly2->cols_get())
	{
		int *pRank	= new int[_pPoly1->size_get()];
		int *pRank1	= new int[_pPoly1->size_get()];
		int *pRank2	= new int[_pPoly2->size_get()];

		_pPoly1->rank_get(pRank1);
		_pPoly2->rank_get(pRank2);
		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			pRank[i] = Max(pRank1[i], pRank2[i]);
		}

		(*_pPolyOut) = new MatrixPoly(_pPoly2->var_get(), _pPoly1->rows_get(), _pPoly1->cols_get(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
		}

		if(bComplex1 == false && bComplex2 == false)
		{
			for(int i = 0 ; i < _pPoly1->size_get() ; i++)
			{
				iAddRealPolyToRealPoly(
					_pPoly1->poly_get(i)->coef_get()->real_get(), pRank1[i],
					_pPoly2->poly_get(i)->coef_get()->real_get(), pRank2[i],
					(*_pPolyOut)->poly_get(i)->coef_get()->real_get(), pRank[i]);
			}
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			for(int i = 0 ; i < _pPoly1->size_get() ; i++)
			{
				iAddRealPolyToComplexPoly(
					_pPoly1->poly_get(i)->coef_get()->real_get(), pRank1[i],
					_pPoly2->poly_get(i)->coef_get()->real_get(), _pPoly2->poly_get(i)->coef_get()->img_get(), pRank2[i],
					(*_pPolyOut)->poly_get(i)->coef_get()->real_get(), (*_pPolyOut)->poly_get(i)->coef_get()->img_get(), pRank[i]);
			}
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			for(int i = 0 ; i < _pPoly1->size_get() ; i++)
			{
				iAddRealPolyToComplexPoly(
					_pPoly2->poly_get(i)->coef_get()->real_get(), pRank2[i],
					_pPoly1->poly_get(i)->coef_get()->real_get(), _pPoly1->poly_get(i)->coef_get()->img_get(), pRank1[i],
					(*_pPolyOut)->poly_get(i)->coef_get()->real_get(), (*_pPolyOut)->poly_get(i)->coef_get()->img_get(), pRank[i]);
			}
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			for(int i = 0 ; i < _pPoly1->size_get() ; i++)
			{
				iAddComplexPolyToComplexPoly(
					_pPoly1->poly_get(i)->coef_get()->real_get(), _pPoly1->poly_get(i)->coef_get()->img_get(), pRank1[i],
					_pPoly2->poly_get(i)->coef_get()->real_get(), _pPoly2->poly_get(i)->coef_get()->img_get(), pRank2[i],
					(*_pPolyOut)->poly_get(i)->coef_get()->real_get(), (*_pPolyOut)->poly_get(i)->coef_get()->img_get(), pRank[i]);
			}
		}
		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else if(bScalar1)
	{//size(p1) == 1
		int *pRank	= new int[_pPoly2->size_get()];
		int *pRank1	= new int[_pPoly2->size_get()];
		int *pRank2	= new int[_pPoly2->size_get()];
		memset(pRank1, 0x00, _pPoly2->size_get() * sizeof(int));

		_pPoly1->rank_get(pRank1);
		_pPoly2->rank_get(pRank2);
		for(int i = 0 ; i < _pPoly2->size_get() ; i++)
		{
			pRank[i] = Max(pRank1[0], pRank2[i]);
		}

		(*_pPolyOut) = new MatrixPoly(_pPoly2->var_get(), _pPoly2->rows_get(), _pPoly2->cols_get(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
		}

		//Result P1(0) + P2(i)
		Double *pCoef1	= _pPoly1->poly_get(0)->coef_get();
		double *p1R			= pCoef1->real_get();
		double *p1I			= pCoef1->img_get();
		for(int i = 0 ; i < _pPoly2->size_get() ; i++)
		{
			Double *pCoef2	= _pPoly2->poly_get(i)->coef_get();
			double *p2R			= pCoef2->real_get();
			double *p2I			= pCoef2->img_get();

			Double *pCoefR	= (*_pPolyOut)->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();
			double *pRI			= pCoefR->img_get();

			for(int j = 0 ; j < Min(pRank1[0], pRank2[i]) ; j++)
			{
				pRR[j] = p1R[j] + p2R[j];
			}

			double *pTemp = (pRank1[0] > pRank2[i] ? p1R : p2R);
			for(int j = Min(pRank1[0], pRank2[i]) ; j < Max(pRank1[0], pRank2[i]) ; j++)
			{
				pRR[j] = pTemp[j];
			}

			if((*_pPolyOut)->isComplex())
			{
				for(int j = 0 ; j < Min(pRank1[0], pRank2[i]) ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
				}

				double *pTemp = (pRank1[0] > pRank2[i] ? p1I : p2I);
				for(int j = Min(pRank1[0], pRank2[i]) ; j < Max(pRank1[0], pRank2[i]); j++)
				{
					pRI[j] = pTemp == NULL ? 0 : pTemp[j];
				}
			}
		}

		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else if(bScalar2)
	{//size(p2) == 1
		int *pRank	= new int[_pPoly1->size_get()];
		int *pRank1	= new int[_pPoly1->size_get()];
		int *pRank2	= new int[_pPoly1->size_get()];
		memset(pRank2, 0x00, _pPoly1->size_get() * sizeof(int));

		_pPoly1->rank_get(pRank1);
		_pPoly2->rank_get(pRank2);
		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			pRank[i] = Max(pRank1[i], pRank2[0]);
		}

		(*_pPolyOut) = new MatrixPoly(_pPoly1->var_get(), _pPoly1->rows_get(), _pPoly1->cols_get(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
		}

		//Result P1(i) + P2(0)
		Double *pCoef2	= _pPoly2->poly_get(0)->coef_get();
		double *p2R			= pCoef2->real_get();
		double *p2I			= pCoef2->img_get();

		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			Double *pCoef1	= _pPoly1->poly_get(i)->coef_get();
			double *p1R			= pCoef1->real_get();
			double *p1I			= pCoef1->img_get();

			Double *pCoefR	= (*_pPolyOut)->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();
			double *pRI			= pCoefR->img_get();

			for(int j = 0 ; j < Min(pRank1[i], pRank2[0]) ; j++)
			{
				pRR[j] = p1R[j] + p2R[j];
			}

			double *pTemp = (pRank1[i] > pRank2[0] ? p1R : p2R);
			for(int j = Min(pRank1[i], pRank2[0]) ; j < Max(pRank1[i], pRank2[0]) ; j++)
			{
				pRR[j] = pTemp[j];
			}

			if((*_pPolyOut)->isComplex())
			{
				for(int j = 0 ; j < Min(pRank1[i], pRank2[0]) ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
				}

				double *pTemp = (pRank1[i] > pRank2[0] ? p1I : p2I);
				for(int j = Min(pRank1[i], pRank2[0]) ; j < Max(pRank1[i], pRank2[0]); j++)
				{
					pRI[j] = pTemp == NULL ? 0 : pTemp[j];
				}
			}
		}

		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else
	{
		return 1;
	}

	if((*_pPolyOut) != NULL)
	{
		(*_pPolyOut)->update_rank();
	}

	//if pResult == NULL -> incompatible dimensions
	return 0;
}

int AddStringToString(String *_pString1, String *_pString2, String **_pStringOut)
{
	bool bScalar1				= _pString1->rows_get() == 1 && _pString1->cols_get() == 1;
	bool bScalar2				= _pString2->rows_get() == 1 && _pString2->cols_get() == 1;


	if(bScalar1)
	{//concat pL with each element of pR
		(*_pStringOut)  = new String(_pString2->rows_get(), _pString2->cols_get());
		int iCommonLen	=	(int)wcslen(_pString1->string_get(0,0));

		for(int i = 0 ; i < _pString2->rows_get() ; i++)
		{
			for(int j = 0 ; j < _pString2->cols_get() ; j++)
			{
				int iLen	= (int)wcslen(_pString2->string_get(i, j));
				wchar_t* psz = new wchar_t[iLen + iCommonLen + 1];
				memset(psz, 0x00, (iLen + iCommonLen + 1) * sizeof(wchar_t));
				memcpy(psz, _pString1->string_get(0,0), iCommonLen * sizeof(wchar_t));
				memcpy(psz + iCommonLen, _pString2->string_get(i, j), iLen * sizeof(wchar_t));
				(*_pStringOut)->string_set(i, j, psz);
				delete[] psz;
			}
		}
	}
	else if(bScalar2)
	{//concat each element of pL with pR
		(*_pStringOut)  = new String(_pString1->rows_get(), _pString1->cols_get());
		int iCommonLen	=	(int)wcslen(_pString2->string_get(0,0));

		for(int i = 0 ; i < _pString1->rows_get() ; i++)
		{
			for(int j = 0 ; j < _pString1->cols_get() ; j++)
			{
				int iLen = (int)wcslen(_pString1->string_get(i, j));
				wchar_t* psz = new wchar_t[iLen + iCommonLen + 1];
				memset(psz, 0x00, (iLen + iCommonLen + 1) * sizeof(wchar_t));

				memcpy(psz, _pString1->string_get(i, j), iLen * sizeof(wchar_t));
				memcpy(psz + iLen, _pString2->string_get(0,0), iCommonLen * sizeof(wchar_t));

				(*_pStringOut)->string_set(i, j, psz);
				delete[] psz;
			}
		}
	}
	else if(_pString1->rows_get() == _pString2->rows_get() && _pString1->cols_get() == _pString2->cols_get())
	{//concat pL with pR element wise
		(*_pStringOut) = new String(_pString1->rows_get(), _pString1->cols_get());
		for(int i = 0 ; i < _pString1->rows_get() ; i++)
		{
			for(int j = 0 ; j < _pString1->cols_get() ; j++)
			{
				int iLenL	= (int)wcslen(_pString1->string_get(i, j));
				int iLenR	= (int)wcslen(_pString2->string_get(i, j));
				wchar_t* psz = new wchar_t[iLenL + iLenR + 1];
				memset(psz, 0x00, (iLenL + iLenR + 1) * sizeof(wchar_t));

				memcpy(psz          , _pString1->string_get(i, j), iLenL * sizeof(wchar_t));
				memcpy(psz + iLenL  , _pString2->string_get(i, j), iLenR * sizeof(wchar_t));

				(*_pStringOut)->string_set(i, j, psz);
				delete[] psz;
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;

}

