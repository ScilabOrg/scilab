/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "basic_functions.h"
}

int getProcessMode(types::typed_list &in, int _iProcess, int _iRef);

using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_sum(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iMode = 0;

    if(in.size() < 1 && in.size() > 3)
    {
        Scierror(999,_("%s: Wrong number of input arguments: %d or %d expected.\n"), L"sum", 1, 2);
        return Function::Error;
    }

    if(in[0]->isDouble() == false && in[0]->isPoly() == false /*&& in[0]->isSparse() == false*/)
    {//call overload
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_sum";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    //don't care of 3rd paramter for double or equivalent type
    if(in.size() >= 2)
    {
         iMode = getProcessMode(in, 1, 0);
    }

    if(in[0]->isDouble())
    {
        Double* pIn = in[0]->getAs<Double>();
        if(pIn->isEmpty())
        {
            if(iMode == 0)
            {
                out.push_back(new Double(0));
            }
            else
            {
                out.push_back(Double::Empty());
            }
        }
        else
        {
            Double* pOut = NULL;
            if(iMode == 0)
            {
                double dblR = 0;
                double dblI = 0;

                for(int i = 0 ; i < pIn->getSize() ; i++)
                {
                    dblR += pIn->get(i);
                }

                pOut = new Double(dblR);
                if(pIn->isComplex())
                {
                    pOut->setComplex(true);
                    for(int i = 0 ; i < pIn->getSize() ; i++)
                    {
                        dblI += pIn->getImg(i);
                    }
                    pOut->setImg(0, dblI);
                }
            }
            else
            {
                int iDims = pIn->getDims();
                int* piDims = new int[iDims];

                for(int i = 0 ; i < iDims ; i++)
                {
                    if(iMode == i + 1)
                    {
                        piDims[i] = 1;
                    }
                    else
                    {
                        piDims[i] = pIn->getDimsArray()[i];
                    }
                }

                pOut = new Double(iDims, piDims);
                pOut->setZeros();

                double* pdblIn  = pIn->get();
                double* pdblOut = pOut->get();

                int* piIndex = new int[iDims];

                for(int i = 0 ; i < pIn->getSize() ; i++)
                {
                    //get value
                    double dbl = pdblIn[i];

                    //get array of dim
                    pIn->getIndexes(i, piIndex);

                    //convert indexes for result
                    piIndex[iMode - 1] = 0;
                    int iIndex = pOut->getIndex(piIndex);
                    pdblOut[iIndex] += dbl;
                }
            }

            out.push_back(pOut);
        }
    }
    else if(in[0]->isPoly())
    {
    }
    //else if(in[0]->isSparse())
    //{
    //}

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
static int getProcessMode(types::typed_list &in, int _iProcess, int _iRef)
{
    int iMode = 0;
    if(in[_iProcess]->isString())
    {
        String* pS = in[_iProcess]->getAs<types::String>();
        if(pS->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
        }

        switch(pS->get(0)[0])
        {
        case 'r' :
            iMode = 1;
            break;
        case 'c' :
            iMode = 2;
            break;
        case '*' :
            iMode = 0;
            break;
        case 'm' :
            iMode = -1;
            break;
        default :
            Scierror(999,_("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), "size", _iProcess + 1, "m" , "*" , "r", "c");
            iMode = -2;
            break;
        }
    }
    else if(in[1]->isDouble() && in[1]->getAs<Double>()->isComplex() == false)
    {
        Double* pD = in[_iProcess]->getAs<Double>();
        if(pD->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
            iMode = -2;
        }

        iMode = static_cast<int>(pD->getReal()[0]);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string or a scalar expected.\n"), "size", 2);
        iMode = -2;
    }

    //special case for -1
    if(iMode == -1)
    {
        iMode = 0;
        if(in[_iRef]->getAsGenericType()->getRows() > 1)
        {
            iMode = 1;
        }
        else if(in[_iRef]->getAsGenericType()->getCols() > 1)
        {
            iMode = 2;
        }
    }

    return iMode;
}
/*--------------------------------------------------------------------------*/
