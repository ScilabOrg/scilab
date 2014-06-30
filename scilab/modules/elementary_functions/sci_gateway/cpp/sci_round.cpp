/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "polynom.hxx"

extern "C"
{
    #include "basic_functions.h"
    #include "Scierror.h"
}


extern "C"
{
    extern double C2F(danints)(double*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_round(types::typed_list &in, int _iRetCount, types::typed_list &out)
{

    types::Double* pDblIn       = NULL;
    types::Double* pDblOut      = NULL;
    types::Polynom* pPolyIn     = NULL;
    types::Polynom* pPolyOut    = NULL;

    if (in.size()!=1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): At least %d expected.\n"), "round", 1);
        return types::Function::Error;
    }
    
    if(in[0]->isPoly())
    {
        pPolyIn = in[0]->getAs<types::Polynom>();
        pPolyOut = pPolyIn->clone()->getAs<types::Polynom>();
        
        types::SinglePoly *pInPoly  = NULL;
        types::SinglePoly *pOutPoly  = NULL;

        double* pReal = NULL;
        double* pImag = NULL;

        for(int compterElem =0;compterElem < pPolyIn->getSize();compterElem++)
        {
            pInPoly = pPolyIn->get(compterElem);
            pDblIn = pInPoly->getCoef();

            pOutPoly = pInPoly->clone()->getAs<types::SinglePoly>();

            pReal = new double[pInPoly->getRank()];
            if(pPolyIn->isComplex())
                pImag = new double[pInPoly->getRank()];

            for(int i =0;i < pInPoly->getRank();i++)
            {
                pReal[i]= C2F(danints)(&pDblIn->getReal()[i]);
                  

                if(pPolyIn->isComplex())
                    pImag[i] = C2F(danints)(&pDblIn->getImg()[i]);
            }
            pOutPoly->setCoef(pReal,pImag);
            pPolyOut->set(compterElem,pOutPoly);

            delete pReal;
            if(pPolyIn->isComplex())
                delete pImag;
            delete pOutPoly;
        }

        out.push_back(pPolyOut);
        return types::Function::OK;
    }
    else if(in[0]->isDouble())
    {
        pDblIn = in[0]->getAs<types::Double>();
        pDblOut = pDblIn->clone()->getAs<types::Double>();

        for(int i =0;i < pDblIn->getSize();i++)
        {

           pDblOut->set(i, C2F(danints)(&pDblIn->get()[i]));

            if(pDblIn->isComplex())
                pDblOut->setImg(i,C2F(danints)(&pDblIn->getImg()[i]));
                
        }

        out.push_back(pDblOut);
        return types::Function::OK;
    }
    else
    {
        Scierror(999, _("%s: Wrong type for argument %d.\n"), "round", 1);
        return types::Function::Error;
    }
    return types::Function::OK;
}

/*int sci_round(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iType 		= 0;
    int* piAddr		= NULL;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddr, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    switch (iType)
    {
        case sci_matrix :
            sciErr = round_double(pvApiCtx, piAddr);
            break;
        case sci_poly :
            sciErr = round_poly(pvApiCtx, piAddr);
            break;
        default:
            OverLoad(1);
            return 0;
            break;
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

SciErr round_double(void* pvApiCtx, int* _piAddress)
{
    SciErr sciErr;
    int i;
    int iRows						= 0;
    int iCols						= 0;

    double *pdblReal		= NULL;
    double *pdblImg			= NULL;
    double *pdblRealRet	= NULL;
    double *pdblImgRet	= NULL;

    if (isVarComplex(pvApiCtx, _piAddress))
    {
        sciErr = getComplexMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        for (i = 0 ; i < iRows * iCols; i++)
        {
            pdblRealRet[i] = C2F(danints)(&pdblReal[i]);
            pdblImgRet[i]	= C2F(danints)(&pdblImg[i]);
        }
    }
    else
    {
        sciErr = getMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        for (i = 0 ; i < iRows * iCols; i++)
        {
            pdblRealRet[i] = C2F(danints)(&pdblReal[i]);
        }
    }

    return sciErr;
}

SciErr round_poly(void* pvApiCtx, int* _piAddress)
{
    SciErr sciErr;
    int i, j;
    int iRows							= 0;
    int iCols							= 0;
    int iLen							= 0;
    int *piCoeff					= NULL;
    char* pstVarName			= NULL;

    double** pdblReal			= NULL;
    double** pdblImg			= NULL;
    double** pdblRealRet	= NULL;
    double** pdblImgRet	= NULL;

    sciErr = getPolyVariableName(pvApiCtx, _piAddress, pstVarName, &iLen);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    pstVarName = (char*)MALLOC(sizeof(char) * (iLen + 1));

    sciErr = getPolyVariableName(pvApiCtx, _piAddress, pstVarName, &iLen);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    if (isVarComplex(pvApiCtx, _piAddress))
    {
        sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL, NULL);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
        sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        pdblReal		= (double**)malloc(sizeof(double*) * iRows * iCols);
        pdblImg			= (double**)malloc(sizeof(double*) * iRows * iCols);
        pdblRealRet	= (double**)malloc(sizeof(double*) * iRows * iCols);
        pdblImgRet	= (double**)malloc(sizeof(double*) * iRows * iCols);

        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
            pdblImg[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
            pdblRealRet[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
            pdblImgRet[i]		= (double*)malloc(sizeof(double) * piCoeff[i]);
        }

        sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        for (i = 0 ; i < iRows * iCols ; i++)
        {
            for (j = 0 ; j < piCoeff[i] ; j++)
            {
                pdblRealRet[i][j] = C2F(danints)(&pdblReal[i][j]);
                pdblImgRet[i][j] = C2F(danints)(&pdblImg[i][j]);
            }
        }

        sciErr = createComplexMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, (const double * const*) pdblRealRet, (const double * const*) pdblImgRet);
        if (sciErr.iErr)
        {
            return sciErr;
        }
    }
    else
    {
        sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
        sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, NULL);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        pdblReal		= (double**)malloc(sizeof(double*) * iRows * iCols);
        pdblRealRet	= (double**)malloc(sizeof(double*) * iRows * iCols);

        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
            pdblRealRet[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
        }

        sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        for (i = 0 ; i < iRows * iCols ; i++)
        {
            for (j = 0 ; j < piCoeff[i] ; j++)
            {
                pdblRealRet[i][j] = C2F(danints)(&pdblReal[i][j]);
            }
        }

        sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, (const double * const*) pdblRealRet);
        if (sciErr.iErr)
        {
            return sciErr;
        }
    }
    return sciErr;
}
*/
/*--------------------------------------------------------------------------*/
