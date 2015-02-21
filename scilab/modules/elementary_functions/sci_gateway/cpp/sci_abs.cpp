/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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
#include "api_scilab.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"


extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "abs.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_abs(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "abs", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "abs", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        api_scilab::Double* pDblIn  = api_scilab::getAsDouble(in[0]);
        api_scilab::Double* pDblOut = new api_scilab::Double(pDblIn->getDims(), pDblIn->getDimsArray());

        double* pdblInR = pDblIn->get();
        double* pdblInI = pDblIn->getImg();
        double* pdblOut = pDblOut->get();

        if (pDblIn->isComplex())
        {
            for (int i = 0 ; i < pDblIn->getSize() ; i++)
            {
                if (ISNAN(pdblInR[i]))
                {
                    pdblOut[i] = pdblInR[i];
                }
                else if (ISNAN(pdblInI[i]))
                {
                    pdblOut[i] = pdblInI[i];
                }
                else
                {
                    pdblOut[i] = dabsz(pdblInR[i], pdblInI[i]);
                }
            }
        }
        else
        {
            for (int i = 0 ; i < pDblIn->getSize() ; i++)
            {
                if (ISNAN(pdblInR[i]))
                {
                    pdblOut[i] = pdblInR[i];
                }
                else
                {
                    pdblOut[i] = dabss(pdblInR[i]);
                }
            }
        }

        out.push_back(api_scilab::getReturnVariable(pDblOut));
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray());
        double* data = NULL;

        if (pPolyIn->isComplex())
        {
            for (int i = 0; i < pPolyIn->getSize(); i++)
            {
                int rank = pPolyIn->get(i)->getRank();
                types::SinglePoly* pSP = new types::SinglePoly(&data, rank);

                for (int j = 0; j < rank + 1; j++)
                {
                    data[j] = dabsz(pPolyIn->get(i)->get()[j], pPolyIn->get(i)->getImg()[j]);
                }

                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
        else
        {
            for (int i = 0; i < pPolyIn->getSize(); i++)
            {
                int rank = pPolyIn->get(i)->getRank();
                types::SinglePoly* pSP = new types::SinglePoly(&data, rank);

                for (int j = 0; j < rank + 1; j++)
                {
                    data[j] = dabss(pPolyIn->get(i)->get()[j]);
                }

                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }

        out.push_back(pPolyOut);
    }
    else if (in[0]->isInt8())
    {
        types::Int8* pIntIn = in[0]->getAs<types::Int8>();
        types::Int8* pIntOut = new types::Int8(pIntIn->getDims(), pIntIn->getDimsArray());

        for (int i = 0 ; i < pIntIn->getSize() ; i++)
        {
            char cInput = pIntIn->get(i);
            pIntOut->set(i, cInput < 0 ? -cInput : cInput);
        }
        out.push_back(pIntOut);
    }
    else if (in[0]->isInt16())
    {
        types::Int16* pIntIn = in[0]->getAs<types::Int16>();
        types::Int16* pIntOut = new types::Int16(pIntIn->getDims(), pIntIn->getDimsArray());

        for (int i = 0 ; i < pIntIn->getSize() ; i++)
        {
            pIntOut->set(i, abs(pIntIn->get(i)));
        }
        out.push_back(pIntOut);
    }
    else if (in[0]->isInt32())
    {
        types::Int32* pIntIn = in[0]->getAs<types::Int32>();
        types::Int32* pIntOut = new types::Int32(pIntIn->getDims(), pIntIn->getDimsArray());

        for (int i = 0 ; i < pIntIn->getSize() ; i++)
        {
            pIntOut->set(i, abs(pIntIn->get(i)));
        }
        out.push_back(pIntOut);
    }
    else if (in[0]->isInt64())
    {
        types::Int64* pIntIn = in[0]->getAs<types::Int64>();
        types::Int64* pIntOut = new types::Int64(pIntIn->getDims(), pIntIn->getDimsArray());

        for (int i = 0 ; i < pIntIn->getSize() ; i++)
        {
            pIntOut->set(i, llabs(pIntIn->get(i)));
        }
        out.push_back(pIntOut);
    }
    else if (in[0]->isUInt8())
    {
        out.push_back(in[0]->getAs<types::UInt8>()->clone());
    }
    else if (in[0]->isUInt16())
    {
        out.push_back(in[0]->getAs<types::UInt16>()->clone());
    }
    else if (in[0]->isUInt32())
    {
        out.push_back(in[0]->getAs<types::UInt32>()->clone());
    }
    else if (in[0]->isUInt64())
    {
        out.push_back(in[0]->getAs<types::UInt64>()->clone());
    }
    else
    {
        ast::ExecVisitor exec;
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_abs";
        return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
