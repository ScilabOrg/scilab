/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "cumprod.hxx"
#include "int.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "basic_functions.h"
}

/*
clear a; nb = 2500; a = rand(nb, nb) * 50; tic(); cumprod(a); toc
clear a; nb = 2500; a = rand(nb, nb) * 50; a = a + a *%i; tic(); cumprod(a); toc
*/
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_cumprod(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn       = NULL;
    types::Double* pDblOut      = NULL;
    types::Polynom* pPolyIn     = NULL;
    types::Polynom* pPolyOut    = NULL;

    int iOrientation    = 0;
    int iOuttype        = 1; // 1 = native | 2 = double (type of output value)

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "cumprod", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "cumprod", 1);
        return types::Function::Error;
    }

    /***** get data *****/
    switch (in[0]->getType())
    {
        case InternalType::ScilabDouble :
            pDblIn = in[0]->getAs<types::Double>();
            break;
        case InternalType::ScilabBool:
            pDblIn = getAsDouble(in[0]->getAs<types::Bool>());
            iOuttype = 2;
            break;
        case InternalType::ScilabPolynom :
            pPolyIn = in[0]->getAs<types::Polynom>();
            break;
        case InternalType::ScilabInt8:
            pDblIn = getAsDouble(in[0]->getAs<types::Int8>());
            break;
        case InternalType::ScilabInt16:
            pDblIn = getAsDouble(in[0]->getAs<types::Int16>());
            break;
        case InternalType::ScilabInt32:
            pDblIn = getAsDouble(in[0]->getAs<types::Int32>());
            break;
        case InternalType::ScilabInt64:
            pDblIn = getAsDouble(in[0]->getAs<types::Int64>());
            break;
        case InternalType::ScilabUInt8:
            pDblIn = getAsDouble(in[0]->getAs<types::UInt8>());
            break;
        case InternalType::ScilabUInt16:
            pDblIn = getAsDouble(in[0]->getAs<types::UInt16>());
            break;
        case InternalType::ScilabUInt32:
            pDblIn = getAsDouble(in[0]->getAs<types::UInt32>());
            break;
        case InternalType::ScilabUInt64:
            pDblIn = getAsDouble(in[0]->getAs<types::UInt64>());
            break;
        default :
            ast::ExecVisitor exec;
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_cumprod";
            return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }

    if (in.size() >= 2)
    {
        if (in[1]->isDouble())
        {
            types::Double* pDbl = in[1]->getAs<types::Double>();

            if (pDbl->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A positive scalar expected.\n"), "cumprod", 2);
                return types::Function::Error;
            }

            iOrientation = static_cast<int>(pDbl->get(0));

            if (iOrientation <= 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A positive scalar expected.\n"), "cumprod", 2);
                return types::Function::Error;
            }
        }
        else if (in[1]->isString())
        {
            types::String* pStr = in[1]->getAs<types::String>();

            if (pStr->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "cumprod", 2);
                return types::Function::Error;
            }

            wchar_t* wcsString = pStr->get(0);

            if (wcscmp(wcsString, L"*") == 0)
            {
                iOrientation = 0;
            }
            else if (wcscmp(wcsString, L"r") == 0)
            {
                iOrientation = 1;
            }
            else if (wcscmp(wcsString, L"c") == 0)
            {
                iOrientation = 2;
            }
            else if (wcscmp(wcsString, L"m") == 0)
            {
                int iDims = 0;
                int* piDimsArray = NULL;

                if (pDblIn)
                {
                    iDims = pDblIn->getDims();
                    piDimsArray = pDblIn->getDimsArray();
                }
                else
                {
                    iDims = pPolyIn->getDims();
                    piDimsArray = pPolyIn->getDimsArray();
                }

                // old function was "mtlsel"
                for (int i = 0; i < iDims; i++)
                {
                    if (piDimsArray[i] > 1)
                    {
                        iOrientation = i + 1;
                        break;
                    }
                }
            }
            else if ((wcscmp(wcsString, L"native") == 0) && (in.size() == 2))
            {
                iOuttype = 1;
            }
            else if ((wcscmp(wcsString, L"double") == 0) && (in.size() == 2))
            {
                iOuttype = 2;
            }
            else
            {
                const char* pstrExpected = NULL;
                if (in.size() == 2)
                {
                    pstrExpected = "\"*\",\"r\",\"c\",\"m\",\"native\",\"double\"";
                }
                else
                {
                    pstrExpected = "\"*\",\"r\",\"c\",\"m\"";
                }

                Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "cumprod", 2, pstrExpected);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix or a string expected.\n"), "cumprod", 2);
            return types::Function::Error;
        }
    }

    if (in.size() == 3)
    {
        if (in[2]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "cumprod", 3);
            return types::Function::Error;
        }

        types::String* pStr = in[2]->getAs<types::String>();

        if (pStr->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "cumprod", 3);
            return types::Function::Error;
        }

        wchar_t* wcsString = pStr->get(0);

        if (wcscmp(wcsString, L"native") == 0)
        {
            iOuttype = 1;
        }
        else if (wcscmp(wcsString, L"double") == 0)
        {
            iOuttype = 2;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: %s or %s expected.\n"), "cumprod", 3, "\"native\"", "\"double\"");
            return types::Function::Error;
        }
    }

    /***** perform operation *****/
    if (pDblIn)
    {
        if (iOrientation > pDblIn->getDims())
        {
            if (in[0]->isDouble())
            {
                pDblOut = pDblIn->clone()->getAs<types::Double>();
            }
            else
            {
                pDblOut = pDblIn;
            }

            if (in[0]->isBool() == false)
            {
                iOuttype = 2;
            }
        }
        else
        {
            pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());
            cumprod(pDblIn, iOrientation, pDblOut);
            if (in[0]->isDouble() == false)
            {
                delete pDblIn;
                pDblIn = NULL;
            }
        }
    }
    else if (pPolyIn)
    {
        iOuttype = 1;
        if (iOrientation > pPolyIn->getDims())
        {
            pPolyOut = pPolyIn->clone()->getAs<types::Polynom>();
        }
        else
        {
            pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray());
            pPolyOut->setComplex(pPolyIn->isComplex());
            cumprod(pPolyIn, iOrientation, pPolyOut);
        }
    }

    /***** set result *****/
    if ((iOuttype == 1) && (in[0]->isDouble() == false))
    {
        switch (in[0]->getType())
        {
            case InternalType::ScilabBool:
            {
                types::Bool* pB = new types::Bool(pDblOut->getDims(), pDblOut->getDimsArray());
                int* p = pB->get();
                double* pd = pDblOut->get();
                int size = pB->getSize();
                for (int i = 0; i < size; ++i)
                {
                    p[i] = pd[i] != 0 ? 1 : 0;
                }
                out.push_back(pB);
                break;
            }
            case InternalType::ScilabPolynom:
            {
                out.push_back(pPolyOut);
                break;
            }
            case InternalType::ScilabInt8:
            {
                out.push_back(toInt<types::Int8, char>(pDblOut));
                break;
            }
            case InternalType::ScilabInt16:
            {
                out.push_back(toInt<types::Int16, short>(pDblOut));
                break;
            }
            case InternalType::ScilabInt32:
            {
                out.push_back(toInt<types::Int32, int>(pDblOut));
                break;
            }
            case InternalType::ScilabInt64:
            {
                out.push_back(toInt<types::Int64, long long>(pDblOut));
                break;
            }
            case InternalType::ScilabUInt8:
            {
                out.push_back(toInt<types::UInt8, unsigned char>(pDblOut));
                break;
            }
            case InternalType::ScilabUInt16:
            {
                out.push_back(toInt<types::UInt16, unsigned short>(pDblOut));
                break;
            }
            case InternalType::ScilabUInt32:
            {
                out.push_back(toInt<types::UInt32, unsigned int>(pDblOut));
                break;
            }
            case InternalType::ScilabUInt64:
            {
                out.push_back(toInt<types::UInt64, unsigned long long>(pDblOut));
                break;
            }
        }

        if (pDblOut)
        {
            delete pDblOut;
        }
    }
    else
    {
        out.push_back(pDblOut);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
