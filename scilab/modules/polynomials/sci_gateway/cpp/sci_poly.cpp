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
#include "polynom.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include <math.h>
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

    extern int C2F(dprxc)(int*, double*, double*);
    extern int C2F(wprxc)(int*, double*, double*, double*, double*);
    extern double C2F(dasum)(int*, double*, int*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_poly(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn    = NULL;
    types::String* pStrName  = NULL;
    types::Polynom* pPolyOut = NULL;

    std::wstring wstrFlag = L"roots"; // roots (default), coeff

    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "poly", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "poly", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_poly";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pDblIn = in[0]->getAs<types::Double>();

    if (in.size() == 3)
    {
        if (in[2]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "poly", 3);
            return types::Function::Error;
        }

        wstrFlag = in[2]->getAs<types::String>()->get(0);
        if (wstrFlag != L"roots" && wstrFlag != L"coeff")
        {
            Scierror(999, _("%s: Wrong value for input argument #%d : %s or %s expected.\n"), "poly", 3, "roots", "coeff");
            return types::Function::Error;
        }
    }

    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "poly", 2);
        return types::Function::Error;
    }

    pStrName = in[1]->getAs<types::String>();
    if (pStrName->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), "poly", 2);
        return types::Function::Error;
    }

    std::wstring wstrName = pStrName->get(0);
    // search blank
    size_t blankpos = wstrName.find_first_of(L" ");
    if ((int)blankpos != -1)
    {
        // blank found
        Scierror(999, _("%s: Wrong value for input argument #%d : Valid variable name expected.\n"), "poly", 2);
        return types::Function::Error;
    }

    // [] case
    if (pDblIn->getSize() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (wstrFlag == L"roots") // roots
    {
        int iOne = 1;
        double* pdblInReal = pDblIn->get();
        int piDimsArray[2] = {1, 1};
        int iSize = pDblIn->getSize();
        int iRanks = iSize + 1;
        pPolyOut = new types::Polynom(wstrName, 2, piDimsArray, &iRanks);
        double* pdblCoefReal = pPolyOut->get(0)->getCoefReal();
        if (pDblIn->isComplex())
        {
            pPolyOut->setComplex(true);
            double* pdblInImg   = pDblIn->getImg();
            double* pdblCoefImg = pPolyOut->get(0)->getCoefImg();
            C2F(wprxc)(&iSize, pdblInReal, pdblInImg, pdblCoefReal, pdblCoefImg);

            // if imaginary part is null, set polynom real
            if (C2F(dasum)(&iRanks, pdblCoefImg, &iOne) == 0)
            {
                pPolyOut->setComplex(false);
            }
        }
        else
        {
            C2F(dprxc)(&iSize, pdblInReal, pdblCoefReal);
        }
    }
    else // coeff
    {
        if (pDblIn->getRows() != 1 && pDblIn->getCols() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A vector expected.\n"), "poly", 1);
            return types::Function::Error;
        }

        int piDimsArray[2] = {1, 1};
        int iRanks = pDblIn->getSize();
        pPolyOut = new types::Polynom(wstrName, 2, piDimsArray, &iRanks);
        pPolyOut->setComplex(pDblIn->isComplex());
        pPolyOut->setCoef(0, pDblIn);
    }

    out.push_back(pPolyOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
