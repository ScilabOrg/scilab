/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*  Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <math.h>
#include <sstream>
#include <string>
#include "string.hxx"
#include "double.hxx"
#include "function.hxx"
#include "string_gw.hxx"
#include "tostring_common.hxx"
#include "printvisitor.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "symbol.hxx"
#include "tlist.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "os_wcsdup.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
}

using namespace types;

static void getMacroString(Macro* _pM, InternalType** _pOut, InternalType** _pIn, InternalType** _pBody)
{
    //get body
    ast::Exp* exp = _pM->getBody();

    std::wostringstream ostr;
    ast::PrintVisitor pv(ostr, false);

    exp->accept(pv);

    wstring wstBody = ostr.str();
    const wchar_t* pwstBody = wstBody.c_str();

    //first loop to find number of lines
    int iLines = 0;
    for (int i = 0 ; i < (int)wcslen(pwstBody) ; i++)
    {
        if (pwstBody[i] == L'\n')
        {
            iLines++;
        }
    }

    String* pBody = new String(iLines, 1);

    //second loop to assign lines to output data
    int iOffset = 0;
    int iIndex = 0;
    for (int i = 0 ; i < (int)wcslen(pwstBody) ; i++)
    {
        if (pwstBody[i] == L'\n')
        {
            int iLen = i - iOffset;
            wchar_t* pwst = new wchar_t[iLen + 1];
            wcsncpy(pwst, pwstBody + iOffset, iLen);
            pwst[iLen] = L'\0';
            pBody->set(iIndex++, pwst);
            delete[] pwst;
            iOffset = i + 1;
        }
    }

    *_pBody = pBody;

    //get inputs
    std::list<symbol::Variable*>* pIn = _pM->inputs_get();

    if (pIn->size() == 0)
    {
        *_pIn = Double::Empty();
    }
    else
    {
        String *pSIn = new String(1, (int)pIn->size());

        list<symbol::Variable*>::iterator itIn = pIn->begin();
        for (int i = 0 ; i < pIn->size() ; i++, itIn++)
        {
            pSIn->set(i, (*itIn)->name_get().name_get().c_str());
        }

        *_pIn = pSIn;
    }

    //get outputs
    list<symbol::Variable*>* pOut = _pM->outputs_get();
    if (pOut->size() == 0)
    {
        *_pOut = Double::Empty();
    }
    else
    {
        String* pSOut = new String(1, (int)pOut->size());

        list<symbol::Variable*>::iterator itOut = pOut->begin();
        for (int i = 0 ; i < pOut->size() ; i++, itOut++)
        {
            pSOut->set(i, (*itOut)->name_get().name_get().c_str());
        }

        *_pOut = pSOut;
    }
}

static void DoubleComplexMatrix2String(std::wostringstream *_postr,  double _dblR, double _dblI)
{
    /*
    if R && !C -> R
    if R && C -> R + Ci
    if !R && !C -> 0
    if(!R aa C	-> Ci
    */
    DoubleFormat dfR, dfI;
    getDoubleFormat(_dblR, &dfR);
    getDoubleFormat(_dblI, &dfI);

    dfR.bPrintPoint = false;
    dfR.bPaddSign = false;
    dfR.iSignLen = 0;

    dfI.bPrintPoint = false;
    dfI.bPaddSign = false;
    dfI.iSignLen = 0;


    if (_dblR == 0)
    {
        //no real part
        if (_dblI == 0)
        {
            //no imaginary part

            //0
            addDoubleValue(_postr, 0, &dfR);
        }
        else
        {
            //imaginary part

            //I
            *_postr  <<  (_dblI < 0 ? L"-" : L"");
            *_postr  <<  L"%i";
            if (fabs(_dblI) != 1)
            {
                //specail case if I == 1 write only %i and not %i*1
                *_postr  <<  L"*";
                addDoubleValue(_postr, fabs(_dblI), &dfI);
            }
        }
    }
    else
    {
        //real part
        if (_dblI == 0)
        {
            //no imaginary part

            //R
            addDoubleValue(_postr, _dblR, &dfR);
        }
        else
        {
            //imaginary part

            //R
            addDoubleValue(_postr, _dblR, &dfR);
            //I
            *_postr  <<  (_dblI < 0 ? L"-%i" : L"+%i");
            if (fabs(_dblI) != 1)
            {
                //special case if I == 1 write only %i and not %i*1
                *_postr  <<  L"*";
                addDoubleValue(_postr, fabs(_dblI), &dfI);
            }
        }
    }
}

Function::ReturnValue sci_string(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "string", 1);
        return Function::Error;
    }

    switch (in[0]->getType())
    {
        case GenericType::ScilabSparse :
        {
            //C=sparse([0 0 4 0 9;0 0 5 0 0;1 3 0 7 0;0 0 6 0 10;2 0 0 8 0]);string(C)
            types::Sparse* pS = in[0]->getAs<Sparse>();
            int iRows = pS->getRows();
            int iCols = pS->getCols();
            bool isComplex = pS->isComplex();
            std::wostringstream ostr;
            std::vector<std::wstring> vect;
            int st;
            string *stemp = new string();


            ostr << "(" << iRows << "," << iCols << ") sparse matrix";

            vect.push_back(ostr.str());
            ostr.str(L"");
            ostr.clear();

            for (int i = 0 ; i < iRows ; i++)
            {
                for (int j = 0 ; j < iCols ; j++)
                {
                    std::wostringstream temp;
                    double real = pS->getReal(i, j);
                    double cplx = 0;
                    if (isComplex)
                    {
                        cplx = pS->getImg(i, j).imag();
                    }

                    if (real || cplx )
                    {
                        temp << L"(" << i + 1 << L"," <<  j + 1 << L")    ";

                        if (real)
                        {
                            temp << pS->getReal(i, j);
                        }

                        if (cplx)
                        {
                            if (real && cplx > 0)
                            {
                                temp << L"+";
                            }
                            else if (cplx < 0)
                            {
                                temp << L"-";
                            }

                            temp  << L"%i*" << std::abs(cplx);
                        }

                        ostr << temp.str();
                        vect.push_back(ostr.str());
                        ostr.str(L"");
                        ostr.clear();
                    }
                }
            }

            types::String* pSt = new String(vect.size(), 1);
            for (int i = 0 ; i < vect.size(); i++)
            {
                pSt->set(i, vect[i].c_str());
            }

            out.push_back(pSt);
            break;
        }

        case GenericType::ScilabInt8 :
        case GenericType::ScilabUInt8 :
        case GenericType::ScilabInt16 :
        case GenericType::ScilabUInt16 :
        case GenericType::ScilabInt32 :
        case GenericType::ScilabUInt32 :
        case GenericType::ScilabInt64 :
        case GenericType::ScilabUInt64 :
        {
            out.push_back(new types::String("not yet implemented"));
            break;
        }
        case GenericType::ScilabDouble :
        {
            types::Double* pDbl = in[0]->getAs<Double>();
            int iDims = pDbl->getDims();
            int* piDimsArray = pDbl->getDimsArray();
            double* pdblReal = pDbl->get();

            // Special case string([]) == []
            if (pDbl->isEmpty())
            {
                out.push_back(Double::Empty());
                return Function::OK;
            }
            else if (piDimsArray[0] == -1 && piDimsArray[1] == -1)
            {
                out.push_back(new String(L""));
                return Function::OK;
            }

            String *pstOutput = new String(iDims, piDimsArray);
            if (pDbl->isComplex())
            {
                double* pdblImg = pDbl->getImg();
                for (int i = 0; i < pDbl->getSize(); ++i)
                {
                    std::wostringstream ostr;
                    DoubleComplexMatrix2String(&ostr, pdblReal[i], pdblImg[i]);
                    pstOutput->set(i, ostr.str().c_str());
                }
            }
            else
            {
                double dblImg  = 0.0;
                for (int i = 0; i < pDbl->getSize(); ++i)
                {
                    std::wostringstream ostr;
                    DoubleComplexMatrix2String(&ostr, pdblReal[i], dblImg);
                    pstOutput->set(i, ostr.str().c_str());
                }
            }

            out.push_back(pstOutput);
            break;
        }
        case GenericType::ScilabString :
        {
            out.push_back(in[0]);
            break;
        }
        case GenericType::ScilabMacroFile :
        {
            if (_iRetCount != 3)
            {
                Scierror(77, _("%s: Wrong number of output argument(s): %d expected.\n"), "string", 3);
                return Function::Error;
            }

            MacroFile* pMF = in[0]->getAs<MacroFile>();
            InternalType* pOut = NULL;
            InternalType* pIn = NULL;
            InternalType* pBody = NULL;

            getMacroString(pMF->getMacro(), &pOut, &pIn, &pBody);

            out.push_back(pOut);
            out.push_back(pIn);
            out.push_back(pBody);
            break;
        }
        case GenericType::ScilabMacro :
        {
            if (_iRetCount != 3)
            {
                Scierror(77, _("%s: Wrong number of output argument(s): %d expected.\n"), "string", 3);
                return Function::Error;
            }

            Macro* pM = in[0]->getAs<Macro>();
            InternalType* pOut = NULL;
            InternalType* pIn = NULL;
            InternalType* pBody = NULL;

            getMacroString(pM, &pOut, &pIn, &pBody);

            out.push_back(pOut);
            out.push_back(pIn);
            out.push_back(pBody);
            break;
        }
        case GenericType::ScilabTList :
        case GenericType::ScilabMList :
        case GenericType::ScilabPolynom :
        {
            std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_string";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
        }
        default :
        {
            std::wostringstream ostr;
            in[0]->toString(ostr);
            out.push_back(new types::String(ostr.str().c_str()));
            break;
        }
    }
    return Function::OK;
}
