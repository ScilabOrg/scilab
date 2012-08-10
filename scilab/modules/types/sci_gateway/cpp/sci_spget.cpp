/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_spget(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pValues = NULL;
    types::Bool* pBools = NULL;

    int iRows           = 0;
    int iCols           = 0;
    bool bComplex       = false;
    int nonZeros        = 0;
    double* pRows       = NULL;
    double* pCols       = NULL;
    double* pNonZeroR   = NULL;
    double* pNonZeroI   = NULL;

    if(in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"spget", 1);
        return types::Function::Error;
    }

    if(in[0]->isSparse())
    {
        types::Sparse* sp = in[0]->getAs<types::Sparse>();
        nonZeros = static_cast<int>(sp->nonZeros());
        pRows = new double[nonZeros * 2];
        sp->outputRowCol(pRows);
        pCols = pRows + nonZeros;

        pNonZeroR = new double[nonZeros];
        pNonZeroI = new double[nonZeros];
        sp->outputValues(pNonZeroR, pNonZeroI);

        iRows = sp->getRows();
        iCols = sp->getCols();
        bComplex = sp->isComplex();
    }
    else if(in[0]->isSparseBool())
    {
        types::SparseBool* sp = in[0]->getAs<types::SparseBool>();
        nonZeros = static_cast<int>(sp->nbTrue());
        pRows = new double[nonZeros * 2];
        sp->outputRowCol(pRows);
        pCols = pRows + nonZeros;

        iRows = sp->getRows();
        iCols = sp->getCols();
        bComplex = sp->isComplex();
    }
    else
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: sparse matrix expected.\n"), L"spget", 1);
        return types::Function::Error;
    }

    if(_iRetCount > 3)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d to %d expected.\n"), L"spget", 1, 3);
        return types::Function::Error;
    }

    types::Double* pCoord = new types::Double(nonZeros, 2);
    if(_iRetCount > 1)
    {
        if(pBools)
        {
            pBools = new types::Bool(nonZeros, 1);
        }
        else
        {
            pValues = new types::Double(nonZeros, 1, bComplex);
        }
    }

    if(pBools)
    {
        for(int i = 0 ; i < nonZeros ; i++)
        {
            pBools->set(i, true);
            pCoord->set(i, 0, pRows[i]);
            pCoord->set(i, 1, pCols[i]);
        }
    }
    else if(pValues)
    {
        if(pValues->isComplex())
        {
            for(int i = 0 ; i < nonZeros ; i++)
            {
                pValues->set(i, pNonZeroR[i]);
                pValues->setImg(i, pNonZeroI[i]);
                pCoord->set(i, 0, pRows[i]);
                pCoord->set(i, 1, pCols[i]);
            }
        }
        else
        {
            for(int i = 0 ; i < nonZeros ; i++)
            {
                pValues->set(i, pNonZeroR[i]);
                pCoord->set(i, 0, pRows[i]);
                pCoord->set(i, 1, pCols[i]);
            }
        }

        delete[] pNonZeroR;
        delete[] pNonZeroI;
    }
    else
    {
        for(int i = 0 ; i < nonZeros ; i++)
        {
            pCoord->set(i, 0, pRows[i]);
            pCoord->set(i, 1, pCols[i]);
        }
    }


//    int iIndex = 0;
//    for(int iCol = 0 ; iCol < iRows ; iCol++)
//    {
//        for(int i = 0 ; i < nonZeros ; i++)
//        {
//            if(pRows[i] == iCol + 1)
//            {//good row
//                pCoord->set(iIndex, 0, pRows[i]);
//                pCoord->set(iIndex, 1, pCols[i]);
//                if(pValues)
//                {
//                    pValues->set(iIndex, pNonZeroR[i]);
//                    if(pValues->isComplex())
//                    {
//                        pValues->setImg(iIndex, pNonZeroI[i]);
//                    }
//                }
//                iIndex++;
//            }
//        }
//    }

    types::Double* pCoordSorted = gsort(pCoord, NULL, 'lr', 'i');
    out.push_back(pCoordSorted);
    if(_iRetCount > 1)
    {
        if(pBools)
        {
            out.push_back(pBools);
        }
        else
        {
            out.push_back(pValues);
        }
    }

    if(_iRetCount > 2)
    {
        types::Double* pSize = new types::Double(1, 2);
        pSize->set(0, iRows);
        pSize->set(1, iCols);
        out.push_back(pSize);
    }
    delete pCoord;
    delete[] pRows;
    return types::Function::OK;
}
