/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "polynomials_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"

    extern double C2F(dasum)(int*, double*, int*);
    extern int C2F(dpodiv)(double*, double*, int*, int*, int*);
    extern int C2F(wpodiv)(double*, double*, double*, double*, int*, int*, int*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_pppdiv(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double* pdblInR[2]  = {NULL, NULL};// real part of denominator and numerator
    double* pdblInI[2]  = {NULL, NULL};// rimaginary part
    bool bDouble        = false;
    bool pbComplex[2]   = {false, false};
    int piRank[2]       = {0, 0}; // rank of denominator and numerator
    int iErr            = 0;
    int iOne            = 1;

    std::wstring wstrName = L"";

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "pppdiv", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "pppdiv", 1, 2);
        return types::Function::Error;
    }

    // get numerator and denominator
    for (int i = 0; i < 2; i++)
    {
        if (in[i]->isDouble())
        {
            if (bDouble)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A polynom expected.\n"), "pppdiv", i + 1);
                return types::Function::Error;
            }

            types::Double* pDblIn = in[i]->getAs<types::Double>();
            if (pDblIn->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "pppdiv", i + 1);
                return types::Function::Error;
            }

            piRank[i] = 1;
            pdblInR[i] = pDblIn->get();
            if (pDblIn->isComplex())
            {
                pbComplex[i] = true;
                pdblInI[i] = pDblIn->getImg();
            }
        }
        else if (in[i]->isPoly())
        {
            types::Polynom* pPolyIn = in[i]->getAs<types::Polynom>();
            if (pPolyIn->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "pppdiv", i + 1);
                return types::Function::Error;
            }

            if (wstrName != L"" && wstrName != pPolyIn->getVariableName())
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A polynom '%ls' expected.\n"), "pppdiv", i + 1, wstrName.c_str());
                return types::Function::Error;
            }

            wstrName = pPolyIn->getVariableName();
            piRank[i] = pPolyIn->getMaxRank();
            pdblInR[i] = pPolyIn->get(0)->getCoefReal();
            if (pPolyIn->isComplex())
            {
                pbComplex[i] = true;
                pdblInI[i] = pPolyIn->get(0)->getCoefImg();
            }
        }
        else
        {
            std::wstring wstFuncName = L"%"  + in[i]->getShortTypeStr() + L"_pppdiv";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }
    }

    // manage the case where the rank of the denominator
    // is less than the rank of numerator. x / x² = 0 , rest = s.
    if (piRank[0] < piRank[1])
    {
        if (_iRetCount == 2)
        {
            out.push_back(in[0]);
        }
        out.push_back(new types::Double(0));
        return types::Function::OK;
    }

    // perform operation and set the result in pdblInR/I[0]
    double* temp = pdblInR[0];
    pdblInR[0] = new double[piRank[0]];
    memcpy(pdblInR[0], temp, piRank[0] * sizeof(double));
    if (pbComplex[0])
    {
        temp = pdblInI[0];
        pdblInI[0] = new double[piRank[0]];
        memcpy(pdblInI[0], temp, piRank[0] * sizeof(double));
    }

    int iDegreeNum = piRank[0] - 1;
    int iDegreeDen = piRank[1] - 1;
    if (pbComplex[0] == false && pbComplex[1] == false)
    {
        C2F(dpodiv)(pdblInR[0], pdblInR[1], &iDegreeNum, &iDegreeDen, &iErr);
    }
    else
    {
        if (pbComplex[0] == false)
        {
            pdblInI[0] = new double[piRank[0]];
            memset(pdblInI[0], 0x00, piRank[0] * sizeof(double));
        }
        else if (pbComplex[1] == false)
        {
            pdblInI[1] = new double[piRank[1]];
            memset(pdblInI[1], 0x00, piRank[1] * sizeof(double));
        }

        C2F(wpodiv)(pdblInR[0], pdblInI[0], pdblInR[1], pdblInI[1], &iDegreeNum, &iDegreeDen, &iErr);
    }

    // after execution : pdblInR[0](1 -> iDegreeDen) is the rest
    //                   pdblInR[0](iDegreeDen -> end) is the coeff
    int iSizeRest      = iDegreeDen;
    int iSizeCoeff     = iDegreeNum - iDegreeDen + 1;
    double* pdblRestR  = pdblInR[0];
    double* pdblCoeffR = pdblInR[0] + iDegreeDen;
    double* pdblRestI  = NULL;
    double* pdblCoeffI = NULL;
    bool bComplex      = pbComplex[0] || pbComplex[1];

    if (bComplex)
    {
        pdblRestI  = pdblInI[0];
        pdblCoeffI = pdblInI[0] + iDegreeDen;
    }

    // compute the real rank
    if (bComplex) // complex case
    {
        for (int i = iSizeCoeff - 1; i >= 0; i--)
        {
            iSizeCoeff--;
            if (std::abs(pdblCoeffR[i]) + std::abs(pdblCoeffI[i]))
            {
                break;
            }
        }

        for (int i = iSizeRest - 1; i >= 0; i--)
        {
            iSizeRest--;
            if (std::abs(pdblRestR[i]) + std::abs(pdblRestI[i]))
            {
                break;
            }
        }
    }
    else // real case
    {
        for (int i = iSizeCoeff - 1; i >= 0; i--)
        {
            iSizeCoeff--;
            if (pdblCoeffR[i])
            {
                break;
            }
        }

        for (int i = iSizeRest - 1; i >= 0; i--)
        {
            iSizeRest--;
            if (pdblRestR[i])
            {
                break;
            }
        }
    }

    // set the rest of divide in output
    if (_iRetCount == 2)
    {
        if (iSizeRest == 0) // return a types::Double
        {
            if (bComplex)
            {
                out.push_back(new types::Double(pdblRestR[0], pdblRestI[0]));
            }
            else
            {
                out.push_back(new types::Double(pdblRestR[0]));
            }
        }
        else // return a types::Polynom
        {
            double* pdblReal = NULL;
            types::Polynom* pPolyOut = new types::Polynom(wstrName, 1, 1);
            types::SinglePoly* pSP = NULL;
            int iSize = iSizeRest + 1;
            if (bComplex && C2F(dasum)(&iSize, pdblRestI, &iOne) != 0)
            {
                double* pdblImg = NULL;
                pSP = new types::SinglePoly(&pdblReal, &pdblImg, iSize);
                memcpy(pdblImg, pdblRestI, iSize * sizeof(double));
            }
            else
            {
                pSP = new types::SinglePoly(&pdblReal, iSize);
            }

            memcpy(pdblReal, pdblRestR, iSize * sizeof(double));
            pPolyOut->set(0, pSP);
            delete pSP;
            out.push_back(pPolyOut);
        }
    }

    // set the result of divide in output
    if (iSizeCoeff == 0) // return a types::Double
    {
        if (bComplex)
        {
            out.push_back(new types::Double(pdblCoeffR[0], pdblCoeffI[0]));
        }
        else
        {
            out.push_back(new types::Double(pdblCoeffR[0]));
        }
    }
    else // return a types::Polynom
    {
        double* pdblReal = NULL;
        types::Polynom* pPolyOut = new types::Polynom(wstrName, 1, 1);
        types::SinglePoly* pSP = NULL;
        int iSize = iSizeCoeff + 1;
        if (bComplex && C2F(dasum)(&iSize, pdblCoeffI, &iOne) != 0)
        {
            double* pdblImg = NULL;
            pSP = new types::SinglePoly(&pdblReal, &pdblImg, iSize);
            memcpy(pdblImg, pdblCoeffI, iSize * sizeof(double));
        }
        else
        {
            pSP = new types::SinglePoly(&pdblReal, iSize);
        }

        memcpy(pdblReal, pdblCoeffR, iSize * sizeof(double));
        pPolyOut->set(0, pSP);
        delete pSP;
        out.push_back(pPolyOut);
    }

    delete pdblInR[0];
    if (pbComplex[0] || pbComplex[1])
    {
        delete pdblInI[0];
    }

    if (pbComplex[1] == false)
    {
        delete pdblInI[1];
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
