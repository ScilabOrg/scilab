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
#include "max.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_max(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::wstring wcsError = L"";

    bool isDouble   = false;
    bool isSparse   = false;

    int iCountElem   = in.size();
    int iDims        = 0;
    int* iDimsArray  = NULL;
    int iOrientation = 0;

    types::Double* pDblIndex    = NULL;
    types::Double* pDblOut      = NULL;
    types::Sparse* pSparseOut   = NULL;

    std::vector<types::InternalType*> inputs;
    std::vector<types::Double*> vectDouble;
    std::vector<types::Sparse*> vectSparse;

    if(in.size() < 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): At least %d expected.\n"), L"max", 1);
        return types::Function::Error;
    }

    if(_iRetCount > 2)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"max", 1, 2);
        return types::Function::Error;
    }

/***** get data *****/
    if(in.size() == 1 && in[0]->isList())
    {
        types::List* pList = in[0]->getAs<types::List>();
        iCountElem = pList->getSize();

        if(iCountElem == 0)
        {
            ScierrorW(999, _W("%ls: Wrong size for input argument #%d: Non empty list expected.\n"), L"max", 1);
            return types::Function::Error;
        }

        for(int i = 0; i < iCountElem; i++)
        {
            inputs.push_back(pList->get(i)->getAs<types::InternalType>());
        }
    }
    else
    {
        inputs = in;
    }

    if(inputs[0]->isDouble())
    {
        wcsError = L"real";
        isDouble = true;
    }
    else if(inputs[0]->isSparse())
    {
        wcsError = L"sparse";
        isSparse = true;
    }
/*    else if(inputs[0]->isInt())
    {
        wcsError = L"real";
        isSparse = false;
    }
*/
    else
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A matrix expected.\n"), L"max", 1);
        return types::Function::Error;
    }

    types::GenericType* pGT = inputs[0]->getAs<types::GenericType>();
    iDims       = pGT->getDims();
    iDimsArray  = pGT->getDimsArray();

    if(pGT->getSize() == 0)
    {
        out.push_back(types::Double::Empty());
        if(_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    if(in.size() == 2)
    {
        if(in[1]->isString())
        {
            std::wstring wcsOrientation = in[1]->getAs<types::String>()->get(0);
            if(wcsOrientation == L"r")
            {
                iOrientation = 1;
            }
            else if(wcsOrientation == L"c")
            {
                iOrientation = 2;
            }
            else if(wcsOrientation == L"m")
            {
                iOrientation = -1;
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d: r, c or m expected.\n"), L"max", 2);
                return types::Function::Error;
            }
            iCountElem = 1;
        }
        else if(in[1]->isDouble())
        {
            types::Double* pDbl = in[1]->getAs<types::Double>();
            if(pDbl->isScalar() && in[0]->getAs<types::Double>()->isScalar() == false)
            {
                iOrientation = pDbl->get(0);
                iCountElem = 1;
            }
        }

        inputs = in;
    }

    if(iOrientation > iDims)
    {
        ScierrorW(999, _W("%ls: Wrong value for input argument #%d: value less than or equal to the number of dimension expected.\n"), L"max", 2);
        return types::Function::Error;
    }

    for(int i = 0; i < iCountElem; i++)
    {
        if(isDouble && inputs[i]->isDouble())
        {
            types::Double* pDbl = inputs[i]->getAs<types::Double>();

            if(pDbl->isComplex())
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A real matrix expected.\n"), L"max", i+1);
                return types::Function::Error;
            }

            if(iDims != pDbl->getDims())
            {
                ScierrorW(999, _W("%ls: Wrong size for input argument #%d: All arguments must have the same size.\n"), L"max", i+1);
                return types::Function::Error;
            }

            int* iCurrentDimsArray = pDbl->getDimsArray();
            for(int iterDims = 0; iterDims < iDims; iterDims++)
            {
                if(iCurrentDimsArray[iterDims] != iDimsArray[iterDims])
                {
                    ScierrorW(999, _W("%ls: Wrong size for input argument #%d: All arguments must have the same size.\n"), L"max", i+1);
                    return types::Function::Error;
                }
            }

            vectDouble.push_back(pDbl);
        }
        else if(isSparse && inputs[i]->isSparse())
        {
            types::Sparse* pSparse = inputs[i]->getAs<types::Sparse>();
            if(pSparse->isComplex())
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A real matrix expected.\n"), L"max", i+1);
                return types::Function::Error;
            }

            if(pSparse->getDims() > 2)
            {
                ScierrorW(999, _W("%ls: Wrong size for input argument #%d: Two dimensional Sparse matrix expected.\n"), L"max", i+1);
                return types::Function::Error;
            }

            if(iDimsArray[0] != pSparse->getRows() || iDimsArray[1] != pSparse->getCols())
            {
                ScierrorW(999, _W("%ls: Wrong size for input argument #%d: All arguments must have the same size.\n"), L"max", i+1);
                return types::Function::Error;
            }

            vectSparse.push_back(pSparse);
        }
/*            else if(isDouble && inputs[i]->isInt())
        {
            types::Double pDbl = new types::Double();
        }
*/
        else
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A %ls matrix expected.\n"), L"max", i+1, wcsError.c_str());
            return types::Function::Error;
        }
    }

/***** perform operation and set result *****/
    int* iSizes = NULL;
    int iSize   = 2;

    if(iCountElem == 1)
    {
        if(iOrientation)
        {
            iSize = iDims;
            iSizes = new int[iDims];
            memcpy(iSizes, iDimsArray, iDims * sizeof(int));
            iSizes[iOrientation-1] = 1;
        }
        else
        {
            iSizes = new int[2];
            iSizes[0] = 1;
            iSizes[1] = 1;
        }
    }
    else
    {
        iSize = iDims;
        iSizes = iDimsArray;
    }

    if(_iRetCount == 2)
    {
        pDblIndex = new types::Double(iSize, iSizes);
    }

    if(isDouble)
    {
        pDblOut = new types::Double(iSize, iSizes);

        max(vectDouble, iOrientation, pDblIndex, pDblOut);
        out.push_back(pDblOut);
    }
    else if(isSparse)
    {
        pSparseOut = new types::Sparse(iSizes[0], iSizes[1]);

        max(vectSparse, iOrientation, pDblIndex, pSparseOut);
        out.push_back(pSparseOut);
    }

    if(_iRetCount == 2 && pDblIndex)
    {
        out.push_back(pDblIndex);
    }

    if(iCountElem == 1)
    {
        delete iSizes;
        iSizes = NULL;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
