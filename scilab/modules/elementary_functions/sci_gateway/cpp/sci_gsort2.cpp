/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "gsort.h"
#include "abs.h" // complex case
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_gsort(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblOut      = NULL;

    int iOption     = 2;
    int iDirection  = 0;
    int iLhs2       = 0;
    int* piInd      = NULL;
    int iRowsInd    = 0;
    int iColsInd    = 0;

    std::string strWay       = "d";
    std::string strProcess   = "g";

    if(in.size() < 1 || in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"gsort", 1, 3);
        return types::Function::Error;
    }

    if(_iRetCount > 2)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"gsort", 1, 2);
        return types::Function::Error;
    }

/***** get data and perform operation *****/
    if(in.size() == 3) // get Direction
    {
        if(in[2]->isString() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A string expected.\n"), L"gsort", 3);
            return types::Function::Error;
        }

        wchar_t* wcsString = in[2]->getAs<types::String>()->get(0);

        if(wcscmp(wcsString, L"i") == 0)
        {
            iDirection = 1;
            strWay = "i";
        }
        else if(wcscmp(wcsString, L"d") == 0)
        {
            iDirection = 0;
            strWay = "d";
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d : It must be one of the following strings : i or d.\n"), L"gsort", 3);
            return types::Function::Error;
        }
    }

    if(in.size() >= 2)  // get Option
    {
        if(in[1]->isString() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A string expected.\n"), L"gsort", 2);
            return types::Function::Error;
        }

        wchar_t* wcsString = in[1]->getAs<types::String>()->get(0);

        if(wcscmp(wcsString, L"r") == 0)
        {
            iOption = 0;
            strProcess = "r";
        }
        else if(wcscmp(wcsString, L"c") == 0)
        {
            iOption = 1;
            strProcess = "c";
        }
        else if(wcscmp(wcsString, L"g") == 0)
        {
            iOption = 2;
            strProcess = "g";
        }
        else if(wcscmp(wcsString, L"lr") == 0)
        {
            iOption = 3;
            strProcess = "lr";
        }
        else if(wcscmp(wcsString, L"lc") == 0)
        {
            iOption = 4;
            strProcess = "lc";
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d : It must be one of the following strings : r, c, g, lr or lc.\n"), L"gsort", 2);

            return types::Function::Error;
        }
    }

    // get data and perform operation for each types::
    if(in[0]->isGenericType() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_matrix";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    types::GenericType* pGTOut = in[0]->getAs<types::GenericType>();

    if(pGTOut->getDims() > 2)
    {
        std::wstring wstFuncName = L"%hm_gsort";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    int iRows = pGTOut->getRows();
    int iCols = pGTOut->getCols();

    iRowsInd = (iOption == 4) ? 1 : iRows;
    iColsInd = (iOption == 3) ? 1 : iCols;

    piInd   = new int[iRowsInd * iColsInd];

    iLhs2 = _iRetCount - 1;

    if(in[0]->isDouble()) // double
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();

        if(pDblIn->isComplex())
        {
            pDblOut = new types::Double(pDblIn->getRows(), pDblIn->getCols(), true);
            for(int i = 0 ; i < pDblIn->getSize() ; i++)
            {
	            pDblOut->set(i, dabsz(pDblIn->get(i), pDblIn->getImg(i)));
            }
        }
        else
        {
            pDblOut = pDblIn->clone()->getAs<types::Double>();
        }

        C2F(gsortd)(pDblOut->get(), piInd, &iLhs2, &iRows, &iCols, (char*)strProcess.c_str(), (char*)strWay.c_str());

        if(pDblIn->isComplex())
        {
            int iPosGet = 0;
            int iPosSet = 0;
            switch(iOption)
            {
                case 0 : // "r"
                {
                    for(int c = 0; c < iCols; c++)
                    {
                        for(int r = 0; r < iRows; r++)
                        {
                            iPosGet = (c * iRows) + (piInd[c * iRows + r] - 1);
                            iPosSet = (c * iRows) + r;
	                        pDblOut->set(iPosSet, pDblIn->get(iPosGet));
	                        pDblOut->setImg(iPosSet, pDblIn->getImg(iPosGet));
                        }
                    }
                    break;
                }
                case 1 : // "c"
                {
                    for(int c = 0; c < iCols; c++)
                    {
                        for(int r = 0; r < iRows; r++)
                        {
                            iPosGet = ((piInd[c * iRows + r] - 1) * iRows) + r;
                            iPosSet = (c * iRows) + r;
	                        pDblOut->set(iPosSet, pDblIn->get(iPosGet));
	                        pDblOut->setImg(iPosSet, pDblIn->getImg(iPosGet));
                        }
                    }
                    break;
                }
                case 2 : // "g"
                {
                    for(int i = 0; i < pDblIn->getSize(); i++)
                    {
	                    pDblOut->set(i, pDblIn->get(piInd[i] - 1));
	                    pDblOut->setImg(i, pDblIn->getImg(piInd[i] - 1));
                    }
                    break;
                }
                case 3 : // "lr"
                {
                    for(int c = 0; c < iCols; c++)
                    {
                        for(int r = 0; r < iRows; r++)
                        {
                            iPosGet = (c * iRows) + (piInd[r] - 1);
                            iPosSet = (c * iRows) + r;
	                        pDblOut->set(iPosSet, pDblIn->get(iPosGet));
	                        pDblOut->setImg(iPosSet, pDblIn->getImg(iPosGet));
                        }
                    }
                    break;
                }
                case 4 : // "lc"
                {
                    for(int c = 0; c < iCols; c++)
                    {
                        for(int r = 0; r < iRows; r++)
                        {
                            iPosGet = ((piInd[c] - 1) * iRows) + r;
                            iPosSet = (c * iRows) + r;
	                        pDblOut->set(iPosSet, pDblIn->get(iPosGet));
	                        pDblOut->setImg(iPosSet, pDblIn->getImg(iPosGet));
                        }
                    }
                    break;
                }
            }
        }
        out.push_back(pDblOut);
    }
    else if(in[0]->isSparse()) // sparse
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        switch(iOption)
        {
            case 0 : // "r"
            {

                break;
            }
            case 1 : // "c"
            {
 
                break;
            }
            case 2 : // "g"
            {

                break;
            }
            case 3 : // "lr"
            {

                break;
            }
            case 4 : // "lc"
            {

                break;
            }
        }
    }
    else if(in[0]->isInt8()) // int
    {
        types::Int8* pIOut = in[0]->getAs<types::Int8>()->clone()->getAs<types::Int8>();
        C2F(gsortchar)(pIOut->get(), piInd, &iLhs2, &iRows, &iCols, (char*)strProcess.c_str(), (char*)strWay.c_str());
        out.push_back(pIOut);
    }
    else if(in[0]->isInt16())
    {
        types::Int16* pIOut = in[0]->getAs<types::Int16>()->clone()->getAs<types::Int16>();
        C2F(gsortshort)(pIOut->get(), piInd, &iLhs2, &iRows, &iCols, (char*)strProcess.c_str(), (char*)strWay.c_str());
        out.push_back(pIOut);
    }
    else if(in[0]->isInt32())
    {
        types::Int32* pIOut = in[0]->getAs<types::Int32>()->clone()->getAs<types::Int32>();
        C2F(gsortint)(pIOut->get(), piInd, &iLhs2, &iRows, &iCols, (char*)strProcess.c_str(), (char*)strWay.c_str());
        out.push_back(pIOut);
    }
    else if(in[0]->isInt64()) // uint
    {
        types::Int64* pIIn = in[0]->getAs<types::Int64>();
        types::Int64* pIOut = new types::Int64(pIIn->getRows(), pIIn->getCols());

        std::multimap<long long, int> sorted;
        std::multimap<long long, int>::iterator it;

        for(int i = 0; i < pIIn->getSize(); i++)
        {
            sorted.insert(std::pair<long long, int>(pIIn->get(i), i));
        }

        int iNewPos = 0;
        for(it = sorted.begin(); it != sorted.end(); it++)
        {
            pIOut->set(iNewPos, it->first);
            piInd[iNewPos] = it->second + 1;
            iNewPos++;
        }

        out.push_back(pIOut);
    }
    else if(in[0]->isUInt8())
    {
        types::UInt8* pIOut = in[0]->getAs<types::UInt8>()->clone()->getAs<types::UInt8>();
        C2F(gsortuchar)(pIOut->get(), piInd, &iLhs2, &iRows, &iCols, (char*)strProcess.c_str(), (char*)strWay.c_str());
        out.push_back(pIOut);
    }
    else if(in[0]->isUInt16())
    {
        types::UInt16* pIOut = in[0]->getAs<types::UInt16>()->clone()->getAs<types::UInt16>();
        C2F(gsortushort)(pIOut->get(), piInd, &iLhs2, &iRows, &iCols, (char*)strProcess.c_str(), (char*)strWay.c_str());
        out.push_back(pIOut);
    }
    else if(in[0]->isUInt32())
    {
        types::UInt32* pIOut = in[0]->getAs<types::UInt32>()->clone()->getAs<types::UInt32>();
        C2F(gsortuint)(pIOut->get(), piInd, &iLhs2, &iRows, &iCols, (char*)strProcess.c_str(), (char*)strWay.c_str());
        out.push_back(pIOut);
    }
    else if(in[0]->isUInt64())
    {

    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_gsort";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

/***** set result *****/
    if(_iRetCount == 2)
    {
        types::Double* pDblInd = new types::Double(iRowsInd, iColsInd);
        for(int i = 0; i < pDblInd->getSize(); i++)
        {
            pDblInd->set(i, static_cast<double>(piInd[i]));
        }

        out.push_back(pDblInd);
    }

    delete piInd;
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
