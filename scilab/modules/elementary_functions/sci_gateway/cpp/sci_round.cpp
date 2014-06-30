/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Sylvain Genin
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
#include "overload.hxx"
#include "execvisitor.hxx"

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

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): At least %d expected.\n"), "round", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "round", 1);
        return types::Function::Error;
    }

    if (in[0]->isPoly())
    {
        /***** get data *****/
        pPolyIn           = in[0]->getAs<types::Polynom>();
        int* piRankPolyIn = new int[pPolyIn->getSize()];
        pPolyIn->getRank(piRankPolyIn);
        pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray(), piRankPolyIn);
        delete piRankPolyIn;

        types::SinglePoly *pOutPoly  = NULL;

        double* pReal = NULL;
        double* pImag = NULL;

        for (int compterElem = 0; compterElem < pPolyIn->getSize(); compterElem++)
        {
            pDblIn = pPolyIn->get(compterElem)->getCoef();

            if (pPolyIn->isComplex())
            {
                pOutPoly = new types::SinglePoly(&pReal, &pImag, pPolyIn->get(compterElem)->getRank());
            }
            else
            {
                pOutPoly = new types::SinglePoly(&pReal, pPolyIn->get(compterElem)->getRank());
            }

            /***** perform operation *****/
            if (pPolyIn->isComplex())
            {
                double* pdblReal = pDblIn->getReal();
                double* pdblImg = pDblIn->getImg();
                for (int i = 0; i < pPolyIn->get(compterElem)->getRank(); i++)
                {
                    pReal[i] = C2F(danints)(&pdblReal[i]);
                    pImag[i] = C2F(danints)(&pdblImg[i]);
                }
            }
            else
            {
                double* pdblReal = pDblIn->getReal();
                for (int i = 0; i < pPolyIn->get(compterElem)->getRank(); i++)
                {
                    pReal[i] = C2F(danints)(&pdblReal[i]);
                }
            }

            pPolyOut->set(compterElem, pOutPoly);

            delete pReal;
            if (pPolyIn->isComplex())
            {
                delete pImag;
            }
            delete pOutPoly;
        }

        /***** return data *****/
        out.push_back(pPolyOut);
        return types::Function::OK;
    }
    else if (in[0]->isDouble())
    {
        /***** get data *****/
        pDblIn  = in[0]->getAs<types::Double>();// double
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

        /***** perform operation *****/
        if (pDblIn->isComplex())
        {
            for (int i = 0; i < pDblIn->getSize(); i++)
            {
                pDblOut->set(i, C2F(danints)(&pDblIn->get()[i]));
                pDblOut->setImg(i, C2F(danints)(&pDblIn->getImg()[i]));
            }

        }
        else
        {
            for (int i = 0; i < pDblIn->getSize(); i++)
            {
                pDblOut->set(i, C2F(danints)(&pDblIn->get()[i]));
            }
        }

        /***** return data *****/
        out.push_back(pDblOut);
        return types::Function::OK;
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_round";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
