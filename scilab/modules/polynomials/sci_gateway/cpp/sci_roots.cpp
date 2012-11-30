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
#include <math.h>
#include "Scierror.h"
#include "localization.h"

    extern double C2F(dasum)(int*, double*, int*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_roots(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::wstring wstrAlgo   = L"e"; // e = eigen (default), f = fast
    types::Double* pDblIn   = NULL;
    types::Double* pDblOut  = NULL;
    types::Polynom* pPolyIn = NULL;

    //    double* pdblOutReal  = NULL;
    //    double* pdblOutImg   = NULL;
    double* pdblInReal   = NULL;
    double* pdblInImg    = NULL;
    double* pdblTempReal = NULL;
    double* pdblTempImg  = NULL;

    int iOne = 1;
    int imOne = -1;
    int iSize = 0;
    bool bComplex = false;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "roots", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "roots", 1);
        return types::Function::Error;
    }

    // get algo type
    if (in.size() == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "roots", 2);
            return types::Function::Error;
        }

        types::String* pStrAlgo = in[1]->getAs<types::String>();
        if (pStrAlgo->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), "roots", 2);
            return types::Function::Error;
        }

        wstrAlgo = pStrAlgo->get(0);
        if (wstrAlgo != L"e" && wstrAlgo != L"f")
        {
            Scierror(999, _("%s: Wrong value for input argument #%d : ""%s"" or ""%s"" expected.\n"), "roots", 2, "e", "f");
            return types::Function::Error;
        }
    }

    if (in[0]->isDouble())
    {
        // for Matlab compatibility root of the vector of coefficients
        pDblIn = in[0]->getAs<types::Double>();
        if (pDblIn->isEmpty())
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        iSize = pDblIn->getSize();

        // old fortran function dtild
        // switch elements of a vector. [1 2 3] => [3 2 1]
        pdblInReal = new double[iSize];
        C2F(dcopy)(iSize, pDblIn->get(), &iOne, pdblInReal, &imOne);
        if (pDblIn->isComplex())
        {
            bComplex = true;
            pdblInImg = new double[iSize];
            C2F(dcopy)(iSize, pDblIn->getImg(), &iOne, pdblInImg, &imOne);
        }
    }
    else if (in[0]->isPoly())
    {
        pPolyIn = in[0]->getAs<types::Polynom>();

        if (pPolyIn->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), "roots", 1);
            return types::Function::Error;
        }

        iSize        = pPolyIn->get(0)->getSize();
        pdblInReal = pPolyIn->get(0)->getCoeffReal();

        if (pDblIn->isComplex())
        {
            bComplex = true;
            pdblInImg = pPolyIn->get(0)->getCoeffImg();
        }
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_roots";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    // If "fast" algo was chosen and polynomial is complex,
    // then produce an error.
    if (wstrAlgo == L"f" && bComplex)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d : If algo is ""%s"", a real is expected.\n"), "roots", 2, "f");
        return types::Function::Error;
    }

    double t = 0;
    while (t == 0)
    {
        iSize--;
        if (iSize < 0)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        t = std::abs(pdblTempReal[iSize]);
        if (bComplex)
        {
            t += std::abs(pdblTempImg[iSize]);
        }
    }

    // If "fast" algo was chosen and polynomial has degree greater than 100,
    // then produce an error.
    if (wstrAlgo == L"f" && iSize > 100)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d : If algo is ""%s"", a degree less than %d expected.\n"), "roots", 2, "f", 100);
        return types::Function::Error;
    }

    if (wstrAlgo == L"f")
    {
        // real polynomial: rpoly algorithm
        // this alg is much more speedy, but it may happens that it gives
        // erroneous results without messages : example
        // roots(%s^31-8*%s^30+9*%s^29+0.995) should have two real roots near
        // 1.355 and 6.65 and the other ones inside a circle centered in 0
        // with radius 1

        pDblOut = new types::Double(iSize + 1, 1, true);
        int iFail = 0;

        double* pdblTempReal = new double[iSize + 1];
        C2F(dcopy)(iSize + 1, pdblInReal, &iOne, pdblTempReal, &imOne);
        C2F(rpoly)(pdblTempReal, &iSize, pDblOut->get(), pDblOut->getImg(), iFail);
        delete pdblTemp;

        if (iFail)
        {
            if (iFail == 1)
            {
                Scierror(999, _("%s: Convergence problem...\n"), "roots");
            }
            else if (iFail == 2)
            {
                Scierror(999, _("%s: Leading coefficient is zero.\n"), "roots");
            }
            else if (iFail == 3)
            {
                Scierror(999, _("%s: Too high degree (max 100).\n"), "roots");
            }

            return types::Function::Error;
        }
    }
    else // wstrAlgo == L"e"
    {
        // Companion matrix method
        if (bComplex)
        {
            double sr = pdblInImg[iSize];
            double* pdblTemp = new double[iSize + 1];
            C2F(dcopy)(iSize + 1, pdblTempImg, &iOne, pdblTemp, &imOne);
        }
        else
        {
            double sr = pdblInReal[iSize];
            C2F(dcopy)(iSize + 1, pdblTempReal, &iOne, pdblTemp, &imOne);
            C2F(dscal)(iSize, -1 / sr, pdblTemp, &ione);

        }
    }

    if (pDblIn)
    {
        delete pdblTempReal;
        if (bComplex)
        {
            delete pdblTempImg;
        }
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
