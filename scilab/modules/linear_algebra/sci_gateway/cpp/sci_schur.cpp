/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "linear_algebra_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "configvariable.hxx"
#include "callable.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
//#include "sas_schur.h"
//#include "dynamic_link.h"
#include "scischur.h"
#include "schurSelect.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_schur(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl[2]          = {NULL, NULL};
    double* pData[2]                = {NULL, NULL};
    types::String* pStr             = NULL;
    types::Double* pDblOut[3]       = {NULL, NULL, NULL};
    double* pDataOut[2]             = {NULL, NULL};
    types::Callable* pFunction      = NULL;
    int iCase                       = 1;
    bool bComplexArgs               = false;
    bool bIsRealStr                 = false;
    bool bIsComplexStr              = false;

// *** check number of input args. ***
    if(in.size() < 1 && in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"schur", 1, 3);
        return types::Function::Error;
    }

// *** check type of input args and get it. ***
    if(in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_schur";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pDbl[0] = in[0]->getAs<types::Double>();

    if(pDbl[0]->getRows() != pDbl[0]->getCols())
    {
        ScierrorW(20, _W("%ls: Wrong type for argument %d: Square matrix expected.\n"), L"schur", 1);
        return types::Function::Error;
    }
    if((pDbl[0]->getRows() == -1) || (pDbl[0]->getCols() == -1)) // manage eye case
    {
        ScierrorW(271,_W("%ls: Size varying argument a*eye(), (arg %d) not allowed here.\n"), L"schur", 1);
        return types::Function::Error;
    }

    if(pDbl[0]->isComplex())
    {
        pData[0] = (double *)oGetDoubleComplexFromPointer(pDbl[0]->getReal(), pDbl[0]->getImg(), pDbl[0]->getSize());
    }
    else
    {
        pDbl[0]  = pDbl[0]->clone()->getAs<types::Double>();
        pData[0] = pDbl[0]->getReal();
    }

    if(in.size() > 1)
    {
        if(in[1]->isDouble())
        {
            pDbl[1] = in[1]->getAs<types::Double>();

            if(pDbl[1]->getRows() != pDbl[1]->getCols())
            {
                ScierrorW(20, _W("%ls: Wrong type for argument %d: Square matrix expected.\n"), L"schur", 2);
                return types::Function::Error;
            }

            if((pDbl[1]->getRows() == -1) || (pDbl[1]->getCols() == -1)) // manage eye case
            {
                ScierrorW(271,_W("%ls: Size varying argument a*eye(), (arg %d) not allowed here.\n"), L"schur", 2);
                return types::Function::Error;
            }

            if(pDbl[1]->isComplex())
            {
                pData[1] = (double *)oGetDoubleComplexFromPointer(pDbl[1]->getReal(), pDbl[1]->getImg(), pDbl[1]->getSize());
            }
            else
            {
                pDbl[1]  = pDbl[1]->clone()->getAs<types::Double>();
                pData[1] = pDbl[1]->getReal();
            }

            iCase = 11;
        }
        else if(in[1]->isString())
        {
            pStr = in[1]->getAs<types::String>();
            bIsRealStr = !wcscmp(pStr->get(0), L"r") || !wcscmp(pStr->get(0), L"real");
            bIsComplexStr = !wcscmp(pStr->get(0), L"comp") || !wcscmp(pStr->get(0), L"complex");

            if(bIsComplexStr)
            {
                pDbl[0]->setComplex(true); // pDbl[0] is a clone of in[0]
                pData[0] = (double *)oGetDoubleComplexFromPointer(pDbl[0]->getReal(), pDbl[0]->getImg(), pDbl[0]->getSize());

            }

            if(bIsRealStr || bIsComplexStr)
            {
                iCase = 1;
            }
            else
            {
                iCase = 12;
            }
        }
        else if(in[1]->isCallable())
        {
            pFunction = in[1]->getAs<types::Callable>();
            iCase = 12;
        }
        else
        {
            std::wstring wstFuncName = L"%"  + in[1]->getShortTypeStr() + L"_schur";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }
    }

    if(in.size() == 3)
    {
        if(in[2]->isString() == false && in[2]->isCallable() == false)
        {
            std::wstring wstFuncName = L"%"  + in[2]->getShortTypeStr() + L"_schur";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }

        if(in[2]->isString())
        {
            pStr = in[2]->getAs<types::String>();
        }
        else
        {
            pFunction = in[2]->getAs<types::Callable>();
        }

        iCase = 112;
    }

// *** check number of output args according the input args. ***

    // iCase represents the type of args input.
    // 1 = double, 2 = string, so 112 = double double string.
    switch(iCase)
    {
        case 1: // double
        {
            if(_iRetCount > 2)
            {
                ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"schur", 1, 2);
                return types::Function::Error;
            }
            break;
        }
        case 11: // double double
        {
            if(_iRetCount != 2 && _iRetCount != 4)
            {
                ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d or %d expected.\n"), L"schur", 2, 4);
                return types::Function::Error;
            }
            break;
        }
        case 12: // double string
        {
            if(_iRetCount < 2 && _iRetCount > 3)
            {
                ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"schur", 2, 3);
                return types::Function::Error;
            }
            break;
        }
        default://case 112: // double double string
        {
            if(_iRetCount > 4) // in doc 5 output args are possible ?!?
            {
                ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"schur", 1, 4);
                return types::Function::Error;
            }
            break;
        }
    }

// *** check form of input args. (empty) ***
    // iCase represents the type of args input.
    // 1 = double, 2 = string, so 112 = double double string.
    switch(iCase)
    {
        case 11: // double double
        case 1: // double
        {
            if(pDbl[0]->getCols() == 0)
            {
                for(int i=0; i<_iRetCount; i++)
                {
                    out.push_back(types::Double::Empty());
                }
                return types::Function::OK;
            }
            break;
        }
        case 12: // double string
        {
            if(pDbl[0]->getCols() == 0)
            {
                types::Double* zero = new types::Double(0);

                for(int i = 0; i < _iRetCount; i++)
                {
                    if(i == 1 && !bIsComplexStr && !bIsRealStr)
                    {
                        out.push_back(zero);
                    }
                    else
                    {
                        out.push_back(types::Double::Empty());
                    }
                }
                return types::Function::OK;
            }
            break;
        }
        case 112: // double double string
        {
            if(pDbl[0]->getCols() == 0)
            {
                types::Double* zero = new types::Double(0);

                for(int i=1; i<_iRetCount; i++)
                {
                    out.push_back(types::Double::Empty());
                }

                if(_iRetCount > 1)
                {
                    out.push_back(zero);
                }
                else
                {
                    out.push_back(types::Double::Empty());
                }

                return types::Function::OK;
            }
            break;
        }
    }

// *** perform operations. ***

    bComplexArgs = pDbl[0]->isComplex() || (pDbl[1] && pDbl[1]->isComplex()) || bIsComplexStr;

    if(bIsRealStr && bComplexArgs)
    {
        ScierrorW(202,_W("%ls: Wrong type for input argument #%d: Real matrix expected.\n"), L"schur", 1);
        return types::Function::Error;
    }

    // case 1 and 11
    for(int i = 0; i < 2; i++)
    {
        pDblOut[i] = new types::Double(pDbl[0]->getCols(), pDbl[0]->getCols(), bComplexArgs);
        if(bComplexArgs)
        {
            pDataOut[i] = (double*)MALLOC(pDbl[0]->getSize() * sizeof(doublecomplex));
        }
        else
        {
            pDataOut[i] = pDblOut[i]->getReal();
        }
    }

    // iCase represents the type of args input.
    // 1 = double, 2 = string, so 112 = double double string.
    switch(iCase)
    {
        case 112: // double double string || double double function
        case 12: // double string || double function
        {
            if(pStr)
            {
         /*       selectPtr = getFunctionByName(bComplexArgs, (pDbl[0] && pDbl[1]), pStr->get(0));
                if(selectPtr.none == NULL)
                {
                    ScierrorW(999,_W("%ls: Subroutine not found: %ls\n"), L"schur", pStr->get(0));
                    return types::Function::Error;
                }
                pDblDim = new types::Double(1,1);*/
            }
            else if(pFunction)
            {
                ConfigVariable::setSchurFunction(pFunction);
                pDblOut[2] = new types::Double(1,1);
            }
            break;
        }
        default:// case 1 and 11
            break;
    }

    int iRet = schurSelect(pDbl, pDblOut, bComplexArgs);

    if(iRet)
    {
        printf("failed !!!  %d",iRet);
        return types::Function::Error;
    }

    // return result(s)
    if(bComplexArgs)
    {
        for(int i = 0; i < 2; i++)
        {
            vGetPointerFromDoubleComplex((doublecomplex*)pDataOut[i], pDblOut[i]->getSize(), pDblOut[i]->getReal(), pDblOut[i]->getImg());
            vFreeDoubleComplexFromPointer((doublecomplex*)pDataOut[i]);
            pDataOut[i] = NULL;
        }
    }

    switch(iCase)
    {
        case 1: // double
        {
            if(_iRetCount == 2)
            {
                out.push_back(pDblOut[0]);
            }

            if(bComplexArgs)
            {
                types::Double* result = new types::Double(pDbl[0]->getRows(), pDbl[0]->getCols(), true);
                vGetPointerFromDoubleComplex((doublecomplex*)pData[0], result->getSize(), result->getReal(), result->getImg());
                vFreeDoubleComplexFromPointer((doublecomplex*)pData[0]);
                pData[0] = NULL;
                out.push_back(result);
            }
            else
            {
                out.push_back(pDbl[0]); //pData[0] has been overwritten by its real Schur form T. 
            }                            //pDbl is a clone in real case.
            break;
        }
        case 11: // double double
        {
            for(int i = 0; i < 2; i++)
            {
                if(bComplexArgs)
                {
                    types::Double* result = new types::Double(pDbl[0]->getCols(), pDbl[0]->getCols(), pDbl[0]->isComplex());
                    vGetPointerFromDoubleComplex((doublecomplex*)pData[i], result->getSize(), result->getReal(), result->getImg());
                    vFreeDoubleComplexFromPointer((doublecomplex*)pData[i]);
                    pData[i] = NULL;
                    out.push_back(result);
                }
                else
                {
                    out.push_back(pDbl[i]);
                }
            }
            if(_iRetCount == 4)
            {
                out.push_back(pDblOut[0]);
                if(_iRetCount > 1)
                {
                    out.push_back(pDblOut[1]);
                }
            }
            break;
        }
        case 12: // double string || double function
        {
            out.push_back(pDblOut[0]);
            out.push_back(pDblOut[2]);
            if(_iRetCount == 3)
            {
                if(bComplexArgs)
                {
                    types::Double* result = new types::Double(pDbl[0]->getRows(), pDbl[0]->getCols(), true);
                    vGetPointerFromDoubleComplex((doublecomplex*)pData[0], result->getSize(), result->getReal(), result->getImg());
                    vFreeDoubleComplexFromPointer((doublecomplex*)pData[0]);
                    pData[0] = NULL;
                    out.push_back(result);
                }
                else
                {
                    out.push_back(pDbl[0]);
                }
            }
            break;
        }
        case 112: // double double string || double double function
        {
            switch(_iRetCount)
            {
                case 0 :
                case 1 : break; // dim
                case 4 : // As Es Z dim
                {
                    for(int i = 0; i < 2; i++)
                    {
                        if(bComplexArgs)
                        {
                            types::Double* result = new types::Double(pDbl[0]->getRows(), pDbl[0]->getCols(), true);
                            vGetPointerFromDoubleComplex((doublecomplex*)pData[i], result->getSize(), result->getReal(), result->getImg());
                            vFreeDoubleComplexFromPointer((doublecomplex*)pData[i]);
                            pData[i] = NULL;
                            out.push_back(result);
                        }
                        else
                        {
                            out.push_back(pDbl[i]);
                        }
                    }
                    out.push_back(pDblOut[1]);
                    break;
                }
                case 3 : // Q Z dim
                {
                    out.push_back(pDblOut[0]);
                }
                case 2 : // Z dim
                {
                    out.push_back(pDblOut[1]);
                    break;
                }
            }
            out.push_back(pDblOut[2]);
            break;
        }
    }
    if(bComplexArgs)
    {
        for(int i=0; i<2; i++)
        {
            if(pData[i])
            {
                vFreeDoubleComplexFromPointer((doublecomplex*)pData[i]);
            }
        }
    }

    ConfigVariable::setSchurFunction(NULL);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
