/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "configvariable.hxx"
#include "callable.hxx"
#include "runvisitor.hxx"

extern "C"
{
    #include "scischur.h"
    #include "schurSelect.h"
}

/*--------------------------------------------------------------------------*/
int schurSelect(types::Double** _pDblIn, types::Double** _pDblOut, bool _bIsComplex)
{
    int info                    = 0;
    int* pBwork                 = NULL;
    int iWorksize               = 0;
    double* pRwork              = NULL;
    doublecomplex* pCplxWork    = NULL;
    int iDim                    = 0;
    int iCols                   = _pDblIn[0]->getCols();
    types::Callable* pCall      = ConfigVariable::getSchurFunction();

    pBwork = (int*)MALLOC((_pDblIn[1] ? 2 : 1) * iCols * sizeof(int));
    if(pBwork == NULL)
    {
        return -1;
    }

    const char* jobL = _pDblOut[0]  ? "V":"N";
    const char* jobR = _pDblOut[1]  ? "V":"N";
    const char* sort = pCall        ? "S":"N";

    if(_pDblIn[1] == NULL && _bIsComplex == false)
    {//dgees
        double* pWR = (double*)MALLOC(iCols * sizeof(double));
        double* pWI = (double*)MALLOC(iCols * sizeof(double));
        pRwork = allocDgeesWorkspace(iCols, &iWorksize);

        if(pWR == NULL || pWI == NULL || pRwork == NULL)
        {
            return -1;
        }

        C2F(dgees)(jobL, sort, schur_dgees, &iCols, _pDblIn[0]->getReal(), &iCols, &iDim, pWR, pWI, _pDblOut[0]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);

	    FREE(pWR);
	    FREE(pWI);
    }

    if(_pDblIn[1] == NULL && _bIsComplex)
    {//zgees
        pCplxWork = allocZgeesWorkspace(iCols, &iWorksize);
    }

    if(_pDblIn[1] && _bIsComplex == false)
    {//dgges
        pRwork = allocDggesWorkspace(iCols, &iWorksize);
    }

    if(_pDblIn[1] && _bIsComplex)
    {//zgges
        pCplxWork = allocZggesWorkspace(iCols, &iWorksize);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
double* allocDgeesWorkspace(int iCols, int* allocated)
{
    int info;
    int query = -1;
    double optim;
    double* ret = NULL;

    C2F(dgees)("V", "N", NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, NULL, &iCols, &optim, &query, NULL, &info);

    *allocated = (int)optim;
    ret = (double*) MALLOC(*allocated * sizeof(double));

    if(!ret)
    {
    *allocated = 3 * iCols;
    ret = (double*) MALLOC(*allocated * sizeof(double));

        if(!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}

doublecomplex* allocZgeesWorkspace(int iCols, int* allocated)
{
    int info;
    int query = -1;
    doublecomplex optim;
    doublecomplex* ret = NULL;

    C2F(zgees)("V", "N", NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, &iCols, &optim, &query, NULL, NULL, &info);

    *allocated = (int)optim.r;
    ret = (doublecomplex*) MALLOC(*allocated * sizeof(doublecomplex));

    if(!ret)
    {
        *allocated = 2 * iCols;
        ret = (doublecomplex*) MALLOC(*allocated * sizeof(doublecomplex));
        if(!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}

double* allocDggesWorkspace(int iCols, int* allocated)
{
    int info;
    int query = -1;
    double optim;
    double* ret = NULL;

    C2F(dgges)("V", "N", "N", NULL, &iCols, NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, NULL, NULL, &iCols, NULL, &iCols, &optim, &query, NULL, &info);

    *allocated = (int)optim;
    ret = (double*)MALLOC(*allocated * sizeof(double));

    if(!ret)
    {
        *allocated = 8 * iCols + 16;
        ret = (double*)MALLOC(*allocated * sizeof(double));

        if(!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}

doublecomplex* allocZggesWorkspace(int iCols, int* allocated)
{
    int info;
    int query = -1;
    doublecomplex optim;
    doublecomplex* ret = NULL;

    C2F(zgges)("V", "V", "N", NULL, &iCols, NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, NULL, &iCols, NULL, &iCols, &optim, &query, NULL, NULL, &info);

    *allocated = (int)optim.r;
    ret = (doublecomplex*) MALLOC(*allocated * sizeof(doublecomplex));

    if(!ret)
    {
        *allocated = 2 * iCols;
        ret = (doublecomplex*) MALLOC(*allocated * sizeof(doublecomplex));
        if(!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}
/*--------------------------------------------------------------------------*/
