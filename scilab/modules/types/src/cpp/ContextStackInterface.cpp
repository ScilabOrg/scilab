/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "int.hxx"
#include "context.hxx"

extern "C"
{
#include "MALLOC.h"
#include "machine.h"
#include "scimem.h"
#include "stack1.h"
#include "stack-c.h"
#include "stack-def.h"
#include "stackTypeVariable.h"
#include "sciprint.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#include "code2str.h"
#undef getType
#undef isComplex
}

#define DEFAULTSTACKSIZE 1000000

namespace types
{

static int convertVarToStackDouble(InternalType* _pIT, int _iPos);
static int convertVarToStackString(InternalType* _pIT, int _iPos);
static int convertVarToStackPoly(InternalType* _pIT, int _iPos);
static int convertVarToStackInt(InternalType* _pIT, int _iPos);

static InternalType* convertStackToVarDouble(int _iPos);
static InternalType* convertStackToVarString(int _iPos);
static InternalType* convertStackToVarPoly(int _iPos);
static InternalType* convertStackToVarInt(int _iPos);

int allocStack(int _iSize)
{
    int ptr = 0;
    return ptr;
}

int freeStack()
{
    return 0;
}

int initStack(int _iSize)
{
    return 0;
}

int convertVarToStack(InternalType* _pIT, int _iPos)
{
    int iRet = 0;
    switch (_pIT->getType())
    {
        case InternalType::RealDouble :
        {
            iRet = convertVarToStackDouble(_pIT, _iPos);
            break;
        }
        case InternalType::RealString :
        {
            iRet = convertVarToStackString(_pIT, _iPos);
            break;
        }
        case InternalType::RealPoly :
        {
            iRet = convertVarToStackPoly(_pIT, _iPos);
            break;
        }
        case InternalType::RealInt8 :
        case InternalType::RealInt16 :
        case InternalType::RealInt32 :
        case InternalType::RealInt64 :
        case InternalType::RealUInt8 :
        case InternalType::RealUInt16 :
        case InternalType::RealUInt32 :
        case InternalType::RealUInt64 :
        {
            iRet = convertVarToStackInt(_pIT, _iPos);
            break;
        }
    }

    return 0;
}

static int convertVarToStackDouble(InternalType* _pIT, int _iPos)
{
    Double* pDbl = _pIT->getAs<Double>();
    int iRows = pDbl->getRows();
    int iCols = pDbl->getCols();
    int iComplex = pDbl->isComplex() ? 1 : 0;
    double* pdblReal = pDbl->get();
    double* pdblImg = pDbl->getImg();

    return C2F(createcvarfromptr)(&_iPos, MATRIX_OF_DOUBLE_DATATYPE, &iComplex,
                                  &iRows, &iCols, pdblReal, pdblImg, 1L);
}

static int convertVarToStackString(InternalType* _pIT, int _iPos)
{
    String* pSt = _pIT->getAs<String>();
    int iRows = pSt->getRows();
    int iCols = pSt->getCols();
    wchar_t** pwstData = pSt->get();
    char** pstData = new char*[iRows * iCols];
    for (int i = 0 ; i < iRows * iCols ; i++)
    {
        pstData[i] = wide_string_to_UTF8(pwstData[i]);
    }

    int iRet = C2F(createvarfromptr)(&_iPos, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, pstData, 1L);
    freeArrayOfString(pstData, iRows * iCols);
    delete[] pstData;

    return iRet;

    return 0;
}

static int convertVarToStackPoly(InternalType* _pIT, int _iPos)
{
    Polynom* pP = _pIT->getAs<Polynom>();
    int iRows = pP->getRows();
    int iCols = pP->getCols();
    int iComplex = pP->isComplex();
    int* piVarName = new int[4];
    int* piRank = NULL;
    int iRankSum = 0;

    std::wstring varName = pP->getVariableName();
    char* pstVarname = wide_string_to_UTF8(varName.c_str());
    for (int i = 0 ; i < 4 ; i++)
    {
        piVarName[i] = 40;
    }

    str2code(piVarName, &pstVarname);
    delete[] pstVarname;

    piRank = new int[iRows * iCols];
    pP->getRank(piRank);

    Double* pCoef = pP->getCoef();
    double* pdblCoefR = pCoef->get();
    double* pdblCoefI = pCoef->getImg();

    CreateCPolyVarFromPtr(_iPos, (int**)&piVarName, iRows, iCols, piRank, pdblCoefR, pdblCoefI);
    delete[] piVarName;
    delete[] piRank;
    return 0;
}

static int convertVarToStackInt(InternalType* _pIT, int _iPos)
{
    return 0;
}


InternalType* convertStackToVar(int _iPos)
{
    InternalType* pIT = NULL;

    int iType = C2F(gettype)(&_iPos);
    switch (iType)
    {
        case sci_matrix :
        {
            pIT = convertStackToVarDouble(_iPos);
            break;
        }

        case sci_strings :
        {
            pIT = convertStackToVarString(_iPos);
            break;
        }

        case sci_poly :
        {
            pIT = convertStackToVarPoly(_iPos);
            break;
        }

        default :
        {
            printf("Unmanaged type %d\n", iType);
            return NULL;
        }
    }
    return pIT;
}

static InternalType* convertStackToVarDouble(int _iPos)
{
    InternalType* pIT = NULL;
    int iRows = 0, iCols = 0, iComplex = 0, iReal = 0, iImg = 0;

    /*get data from stack*/
    C2F(getmat)("convertStackToVarDouble", &Top, &_iPos, &iComplex, &iRows, &iCols, &iReal, &iImg, (int)strlen("convertStackToVarDouble"));

    Double* pDbl = NULL;

    /*create new variable*/
    if (iComplex)
    {
        pDbl = new Double(iRows, iCols, true);
        pDbl->set(stk(iReal));
        pDbl->setImg(stk(iImg));
    }
    else
    {
        pDbl = new Double(iRows, iCols);
        pDbl->set(stk(iReal));
    }

    /*return new variable*/
    return pDbl;
}

static InternalType* convertStackToVarString(int _iPos)
{
    InternalType* pIT = NULL;
    int iRows = 0, iCols = 0, iComplex = 0;
    char** pstData = NULL;
    wchar_t** pwstData = NULL;

    /*get data from stack*/
    C2F(getrhsvar)(&_iPos, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, (int*)&pstData, 1L);

    /*convert data from stack to scilab 6 format (char* -> wchar_t*)*/
    pwstData = new wchar_t*[iRows * iCols];
    for (int i = 0 ; i < iRows * iCols ; i++)
    {
        pwstData[i] = to_wide_string(pstData[i]);
    }

    /*create new variable*/
    String* pSt = new String(iRows, iCols, pwstData);

    /*release temporary data*/
    freeArrayOfWideString(pwstData, iRows * iCols);
    delete[] pwstData;

    /*return new variable*/
    return pSt;
}

static InternalType* convertStackToVarPoly(int _iPos)
{
    int iRows = 0, iCols = 0, *piVarname, *piRank = NULL, iReal = 0, iImg = 0;
    char* pstVarname = new char[5];
    wchar_t* pwstVarname = NULL;

    for (int i = 0 ; i < 4 ; i++)
    {
        pstVarname[i] = 40;
    }

    pstVarname[4] = 0;
    piRank = new int[iRows * iRows];
    GetRhsCPolyVar(_iPos, &piVarname, &iRows, &iCols, piRank, &iReal, &iImg);
    code2str((char**)&pstVarname, piVarname, 4);
    pwstVarname = to_wide_string(pstVarname);

    Polynom* pP = new Polynom(pwstVarname, iRows, iCols, piRank);
    delete[] pstVarname;
    FREE(pwstVarname);
    if (iImg)
    {
        pP->setComplex(true);
    }

    int iOffset = 0;
    for (int i = 0 ; i < iRows * iCols ; i++)
    {
        int iRank = piRank[i];
        Double* pCoef = new Double(1, iRank, pP->isComplex());
        pCoef->set(stk(iReal) + iOffset);
        if (pP->isComplex())
        {
            pCoef->setImg(stk(iImg) + iOffset);
        }

        pP->setCoef(i, pCoef);

        iOffset += iRank;
    }

    delete piRank;
    return pP;
}

static InternalType* convertStackToVarInt(int _iPos)
{
    InternalType* pIT = NULL;
    return pIT;
}
} //namespace

