/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "cacsd_gw.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"

extern void C2F(residu)(double*, int*, double*, int*, double*, int*, double*, double*, int*);
extern void C2F(wesidu)(double*, double*, int*, double*, double*, int*,
                        double*, double*, int*, double*, double*, double*, int*);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_residu(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRows[3]             = {0, 0, 0};
    int iCols[3]             = {0, 0, 0};
    int iComplex[3]          = {0, 0, 0};
    int* piRank[3]           = {NULL, NULL, NULL};
    double** pdblInR[3]      = {NULL, NULL, NULL};
    double** pdblInI[3]      = {NULL, NULL, NULL};
    bool isDeletable[3]      = {false, false, false};
    types::Double* pDblIn[3] = {NULL, NULL, NULL};
    types::Polynom* pPoly[3] = {NULL, NULL, NULL};
    types::Double* pDblOut   = NULL;

    char cP         = 'p';
    double dblEps   = C2F(dlamch)(&cP, 1L);
    double dZero    = 0;
    int iOne        = 1;
    int iSize       = 0;
    int iError      = 0;

    if (in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "residu", 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "residu", 1);
        return types::Function::Error;
    }

    try
    {
        /*** get inputs arguments ***/
        for(int i = 0; i < 3; i++)
        {
            if (in[i]->isDouble())
            {
                pDblIn[i] = in[i]->clone()->getAs<types::Double>();
                iRows[i] = pDblIn[i]->getRows();
                iCols[i] = pDblIn[i]->getCols();

                iSize = pDblIn[i]->getSize();
                piRank[i] = new int[iSize];
                memset(piRank[i], 0x00, iSize * sizeof(int));

                pdblInR[i]  = new double*[iSize];
                double* pdbl = pDblIn[i]->get();
                for(int j = 0; j < iSize; i++)
                {
                    pdblInR[i][j] = pdbl + j;
                }

                if (pDblIn[i]->isComplex())
                {
                    pdblInI[i]  = new double*[iSize];
                    double* pdbl = pDblIn[i]->get();
                    for(int j = 0; j < iSize; i++)
                    {
                        pdblInI[i][j] = pdbl + j;
                    }
                }
            }
            else if (in[i]->isPoly())
            {
                pPoly[i] = in[i]->clone()->getAs<types::Polynom>();
                iRows[i] = pPoly[i]->getRows();
                iCols[i] = pPoly[i]->getCols();

                iSize = pPoly[i]->getSize();
                piRank[i] = new int[iSize];
                pPoly[i]->getRank(piRank[i]);

                pdblInR[i] = new double*[iSize];
                for(int j = 0; j < iSize; j++)
                {
                    pdblInR[i][j] = pPoly[i]->get(j)->getCoef()->get();
                }

                if (pPoly[i]->isComplex())
                {
                    pdblInI[i] = new double*[iSize];
                    for(int j = 0; j < iSize; j++)
                    {
                        pdblInI[i][j] = pPoly[i]->get(j)->getCoef()->getImg();
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A Matrix or polynom expected.\n"), "residu", i+1);
                throw 1;
            }
        }

        if(iRows[0] != iRows[1] || iCols[0] != iCols[1] || iRows[0] != iRows[2] || iCols[0] != iCols[2])
        {
            Scierror(999, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "residu");
            throw 1;
        }

        /*** perform operations ***/
        if(pdblInI[0] == NULL && pdblInI[1] == NULL && pdblInI[2] == NULL)
        {// real case
            pDblOut = new types::Double(iRows[0], iCols[0]);
            double* pdblOut = pDblOut->get();
            for(int i = 0; i < iRows[0] * iCols[0]; i++)
            {
                int iErr = 0;
                double v = 0;

                C2F(residu)(pdblInR[0][i], (piRank[0])+i, pdblInR[1][i], (piRank[1])+i,
                            pdblInR[2][i], (piRank[2])+i, &v, &dblEps, &iErr);
                if(iErr)
                {
                    Scierror(78, _("%s: An error occured in '%s'.\n"), "residu", "residu");
                    throw iErr;
                }

                pdblOut[i] = v;
            }
        }
        else
        {// complex case
            pDblOut = new types::Double(iRows[0], iCols[0], true);
            double* pdblOutR = pDblOut->get();
            double* pdblOutI = pDblOut->getImg();

            for(int i= 0; i < 3; i++)
            {
                if(pdblInI[i] == NULL)
                {
                    pdblInI[i] = new double*[iSize];
                    for(int j = 0; j < iSize; j++)
                    {
                        int iLen = piRank[i][j] + 1;
                        pdblInI[i][j] = new double[iLen];
                        memset(pdblInI[i][j], 0x00, iLen * sizeof(double));
                    }

                    isDeletable[i] = true;
                }
            }

            for(int i = 0; i < iRows[0] * iCols[0]; i++)
            {
                int iErr    = 0;
                double real = 0;
                double imag = 0;

                C2F(wesidu)(pdblInR[0][i], pdblInI[0][i], (piRank[0])+i,
                            pdblInR[1][i], pdblInI[1][i], (piRank[1])+i,
                            pdblInR[2][i], pdblInI[2][i], (piRank[2])+i,
                            &real, &imag, &dblEps, &iErr);

                if(iErr)
                {
                    Scierror(78, _("%s: An error occured in '%s'.\n"), "residu", "wesidu");
                    throw iErr;
                }

                pdblOutR[i] = real;
                pdblOutI[i] = imag;
            }
        }
    }
    catch(int error)
    {
        iError = error;
    }

    // free memory
    for(int i = 0; i < 3; i++)
    {
        if(pDblIn[i])
        {
            delete pDblIn[i];
        }

        if(pPoly[i])
        {
            delete pPoly[i];
        }

        if(piRank[i])
        {
            delete[] piRank[i];
        }

        if(pdblInR[i])
        {
            delete[] pdblInR[i];
        }

        if(isDeletable[i])
        {
            for(int j = 0; j < iSize; j++)
            {
                delete[] pdblInI[i][j];
            }
        }

        if(pdblInI[i])
        {
            delete[] pdblInI[i];
        }
    }

    /*** retrun output arguments ***/
    if(iError)
    {
        return types::Function::Error;
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
