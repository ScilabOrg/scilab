/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "basic_functions.h"
#include "Scierror.h"
}


/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_sign(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int i;
    int iRows = 0;
    int iCols = 0;
    int iType = 0;
    int* piAddr = NULL;

    double *pdblReal = NULL;
    double *pdblImg	= NULL;
    double *pdblRealRet	= NULL;
    double *pdblImgRet	= NULL;


    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "sign", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "sign", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();
        int dims  = pDblIn->getDims();
        int *dimsArray = pDblIn->getDimsArray();

        if (pDblIn->isComplex())
        {
            types::Double* pOut = new types::Double(dims, dimsArray, true);
            double *outR = pDblIn->getReal();
            double *outImg = pDblIn->getImg() ;
            for (i = 0 ; i < pOut-> getSize() ; i++)
            {
                double dblTemp = dpythags(pDblIn->getReal()[i], pDblIn->getImg()[i]);
                if (dblTemp == 0)
                {
                    outR[i] = 0;
                    outImg[i] = 0;

                }
                else
                {
                    outR[i] /=  dblTemp;
                    outImg[i] /= dblTemp;
                }
            }
            pOut->set(outR);
            pOut->setImg(outImg);

            out.push_back(pOut);
        }
        else
        {
            types::Double* pOut = new types::Double(dims, dimsArray);

            for (i = 0 ; i < pOut-> getSize() ; i++)
            {
                pOut->set(i, dsignsEx(pDblIn->getReal()[i]));
            }
            out.push_back(pOut);
        }
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_sign";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }



    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
