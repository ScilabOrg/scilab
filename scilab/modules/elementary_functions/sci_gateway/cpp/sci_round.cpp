/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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

    if (in[0]->isPoly())
    {
        pPolyIn = in[0]->getAs<types::Polynom>();
        pPolyOut = pPolyIn->clone()->getAs<types::Polynom>();

        types::SinglePoly *pInPoly  = NULL;
        types::SinglePoly *pOutPoly  = NULL;

        double* pReal = NULL;
        double* pImag = NULL;

        for (int compterElem = 0; compterElem < pPolyIn->getSize(); compterElem++)
        {
            pInPoly = pPolyIn->get(compterElem);
            pDblIn = pInPoly->getCoef();

            pOutPoly = pInPoly->clone()->getAs<types::SinglePoly>();

            pReal = new double[pInPoly->getRank()];
            if (pPolyIn->isComplex())
            {
                pImag = new double[pInPoly->getRank()];
            }

            for (int i = 0; i < pInPoly->getRank(); i++)
            {
                pReal[i] = C2F(danints)(&pDblIn->getReal()[i]);


                if (pPolyIn->isComplex())
                {
                    pImag[i] = C2F(danints)(&pDblIn->getImg()[i]);
                }
            }
            pOutPoly->setCoef(pReal, pImag);
            pPolyOut->set(compterElem, pOutPoly);

            delete pReal;
            if (pPolyIn->isComplex())
            {
                delete pImag;
            }
            delete pOutPoly;
        }

        out.push_back(pPolyOut);
        return types::Function::OK;
    }
    else if (in[0]->isDouble())
    {
        pDblIn = in[0]->getAs<types::Double>();
        pDblOut = pDblIn->clone()->getAs<types::Double>();

        for (int i = 0; i < pDblIn->getSize(); i++)
        {

            pDblOut->set(i, C2F(danints)(&pDblIn->get()[i]));

            if (pDblIn->isComplex())
            {
                pDblOut->setImg(i, C2F(danints)(&pDblIn->getImg()[i]));
            }

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
/*--------------------------------------------------------------------------*/
