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
#include "mlist.hxx"
#include "tlist.hxx"
#include "bool.hxx"
#include "context.hxx"

extern "C"
{
#include "MALLOC.h"
#include "machine.h"
#include "sciprint.h"
#include "charEncoding.h"
#include "stackapi_scilab.h"
#include "freeArrayOfString.h"
#undef getType
#undef isComplex
}

#define DEFAULTSTACKSIZE 1000000

namespace types
{

    static int convertContextToStackDouble(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos);
    static int convertContextToStackString(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos);
    static int convertContextToStackPoly(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos);
    static int convertContextToStackInt(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos);
    static int convertContextToStackBool(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos);
    static int convertContextToStackList(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos);
    static int convertContextToStackData(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos);

    static InternalType* convertStackToContextDouble(int* _piAddr);
    static InternalType* convertStackToContextString(int* _piAddr);
    static InternalType* convertStackToContextPoly(int* _piAddr);
    static InternalType* convertStackToContextInt(int* _piAddr);
    static InternalType* convertStackToContextBool(int* _piAddr);
    static InternalType* convertStackToContextList(int* _piAddr);
    static InternalType* convertStackToContextData(int* _piAddr);

    int convertContextToStack(InternalType* _pIT, int _iPos)
    {
        return convertContextToStackData(_pIT, NULL, 0, _iPos);
    }

    static int convertContextToStackData(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos)
    {
        int iRet = 0;
        switch (_pIT->getType())
        {
        case InternalType::RealDouble :
            {
                iRet = convertContextToStackDouble(_pIT, _piAddress, _iIndex, _iPos);
                break;
            }
        case InternalType::RealString :
            {
                iRet = convertContextToStackString(_pIT, _piAddress, _iIndex, _iPos);
                break;
            }
        case InternalType::RealPoly :
            {
                iRet = convertContextToStackPoly(_pIT, _piAddress, _iIndex, _iPos);
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
                iRet = convertContextToStackInt(_pIT, _piAddress, _iIndex, _iPos);
                break;
            }
        case InternalType::RealList :
        case InternalType::RealTList :
        case InternalType::RealMList :
            {
                iRet = convertContextToStackList(_pIT, _piAddress, _iIndex, _iPos);
                break;
            }
        default :
            {
                printf("convertStackToContextData Unmanaged type %d\n", _pIT->getTypeStr());
            }
        }

        return 0;
    }

    static int convertContextToStackDouble(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos)
    {
        Double* pDbl = _pIT->getAs<Double>();
        int iRows = pDbl->getRows();
        int iCols = pDbl->getCols();
        int iComplex = pDbl->isComplex() ? 1 : 0;
        double* pdblReal = pDbl->get();
        double* pdblImg = pDbl->getImg();
        int iSize = pDbl->getSize();
        int iOne = 1;

        if(_piAddress)
        {
            if(iComplex)
            {
                stackapi_createComplexMatrixOfDoubleInList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, pdblReal, pdblImg);
            }
            else
            {
                stackapi_createMatrixOfDoubleInList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, pdblReal);
            }
        }
        else
        {
            if(iComplex)
            {
                stackapi_createComplexMatrixOfDouble(stackapi_pvApiCtx, _iPos, iRows, iCols, pdblReal, pdblImg);
            }
            else
            {
                stackapi_createMatrixOfDouble(stackapi_pvApiCtx, _iPos, iRows, iCols, pdblReal);
            }
        }

        return 0;
    }

    static int convertContextToStackString(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos)
    {
        int iRet = 0;
        String* pS = _pIT->getAs<String>();
        int iRows = pS->getRows();
        int iCols = pS->getCols();
        wchar_t** pwstData = pS->get();
        char** pstData = (char**)MALLOC(sizeof(char*) * iRows * iCols);

        for (int i = 0 ; i < iRows * iCols ; i++)
        {
            pstData[i] = wide_string_to_UTF8(pwstData[i]);
        }

        if(_piAddress)
        {
            stackapi_createMatrixOfStringInList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, pstData);
        }
        else
        {
            stackapi_createMatrixOfString(stackapi_pvApiCtx, _iPos, iRows, iCols, pstData);
        }

        freeArrayOfString(pstData, iRows * iCols);
        return iRet;
    }

    static int convertContextToStackPoly(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos)
    {
        Polynom* pP = _pIT->getAs<Polynom>();
        int iRows = pP->getRows();
        int iCols = pP->getCols();
        int iComplex = pP->isComplex();
        int* piVarName = (int*)MALLOC(sizeof(int) * 4);
        int* piRank = NULL;
        double* pdblOutR = NULL;
        double* pdblOutI = NULL;

        std::wstring varName = pP->getVariableName();
        char* pstVarname = wide_string_to_UTF8(varName.c_str());

        piRank = (int*)MALLOC(sizeof(int) * iRows * iCols);
        pP->getRank(piRank);

        int iSize = pP->getSize();
        double** pdblCoefR = (double**)MALLOC(sizeof(double*) * iSize);
        double** pdblCoefI = NULL;
        if(iComplex)
        {
            pdblCoefI = (double**)MALLOC(sizeof(double*) * iSize);
        }

        for(int i = 0 ; i < iSize ; i++)
        {
            pdblCoefR[i] = pP->get(i)->getCoefReal();
            if(iComplex)
            {
                pdblCoefI[i] = pP->get(i)->getCoefImg();
            }
        }

        if(_piAddress)
        {//list item
            if(iComplex)
            {
                stackapi_createComplexMatrixOfPolyInList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, pstVarname, iRows, iCols, piRank, pdblCoefR, pdblCoefI);
            }
            else
            {
                stackapi_createMatrixOfPolyInList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, pstVarname, iRows, iCols, piRank, pdblCoefR);
            }
        }
        else
        {
            if(iComplex)
            {
                stackapi_createComplexMatrixOfPoly(stackapi_pvApiCtx, _iPos, pstVarname, iRows, iCols, piRank, pdblCoefR, pdblCoefI);
            }
            else
            {
                stackapi_createMatrixOfPoly(stackapi_pvApiCtx, _iPos, pstVarname, iRows, iCols, piRank, pdblCoefR);
            }
        }

        FREE(piVarName);
        FREE(piRank);
        FREE(pdblCoefR);
        if(iComplex)
        {
            FREE(pdblCoefI);
        }
        return 0;
    }

    template <typename T>
    static int readStackInt(InternalType* _pIT, int* _piRows, int* _piCols, void** _pvData)
    {
        T* pI = _pIT->getAs<T>();
        *_piRows = pI->getRows();
        *_piCols = pI->getCols();
        *_pvData = pI->get();
        return 0;
    }

    static int convertContextToStackInt(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos)
    {
        int iRows = 0;
        int iCols = 0;
        void* pvData = NULL;

        switch(_pIT->getType())
        {
        case InternalType::RealInt8 :
            {
                readStackInt<Int8>(_pIT, &iRows, &iCols, &pvData);
                if(_piAddress)
                {
                    stackapi_createMatrixOfInteger8InList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, (const char*)pvData);
                }
                else
                {
                    stackapi_createMatrixOfInteger8(stackapi_pvApiCtx, _iPos, iRows, iCols, (const char*)pvData);
                }
            }
        case InternalType::RealUInt8 :
            {
                readStackInt<UInt8>(_pIT, &iRows, &iCols, &pvData);
                if(_piAddress)
                {
                    stackapi_createMatrixOfUnsigedInteger8InList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, (const unsigned char*)pvData);
                }
                else
                {
                    stackapi_createMatrixOfUnsigedInteger8(stackapi_pvApiCtx, _iPos, iRows, iCols, (const unsigned char*)pvData);
                }
            }
        case InternalType::RealInt16 :
            {
                readStackInt<Int16>(_pIT, &iRows, &iCols, &pvData);
                if(_piAddress)
                {
                    stackapi_createMatrixOfInteger16InList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, (const short*)pvData);
                }
                else
                {
                    stackapi_createMatrixOfInteger16(stackapi_pvApiCtx, _iPos, iRows, iCols, (const short*)pvData);
                }
            }
        case InternalType::RealUInt16 :
            {
                readStackInt<UInt16>(_pIT, &iRows, &iCols, &pvData);
                if(_piAddress)
                {
                    stackapi_createMatrixOfUnsigedInteger16InList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, (const unsigned short*)pvData);
                }
                else
                {
                    stackapi_createMatrixOfUnsigedInteger16(stackapi_pvApiCtx, _iPos, iRows, iCols, (const unsigned short*)pvData);
                }
            }
        case InternalType::RealInt32 :
            {
                readStackInt<Int32>(_pIT, &iRows, &iCols, &pvData);
                if(_piAddress)
                {
                    stackapi_createMatrixOfInteger32InList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, (const int*)pvData);
                }
                else
                {
                    stackapi_createMatrixOfInteger32(stackapi_pvApiCtx, _iPos, iRows, iCols, (const int*)pvData);
                }
            }
        case InternalType::RealUInt32 :
            {
                readStackInt<UInt32>(_pIT, &iRows, &iCols, &pvData);
                if(_piAddress)
                {
                    stackapi_createMatrixOfUnsigedInteger32InList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, (const unsigned int*)pvData);
                }
                else
                {
                    stackapi_createMatrixOfUnsigedInteger32(stackapi_pvApiCtx, _iPos, iRows, iCols, (const unsigned int*)pvData);
                }
            }
        case InternalType::RealInt64 :
            {
                readStackInt<Int64>(_pIT, &iRows, &iCols, &pvData);
                if(_piAddress)
                {
                    stackapi_createMatrixOfInteger64InList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, (const long long*)pvData);
                }
                else
                {
                    stackapi_createMatrixOfInteger64(stackapi_pvApiCtx, _iPos, iRows, iCols, (const long long*)pvData);
                }
            }
        case InternalType::RealUInt64 :
            {
                readStackInt<UInt64>(_pIT, &iRows, &iCols, &pvData);
                if(_piAddress)
                {
                    stackapi_createMatrixOfUnsigedInteger64InList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, (const unsigned long long*)pvData);
                }
                else
                {
                    stackapi_createMatrixOfUnsigedInteger64(stackapi_pvApiCtx, _iPos, iRows, iCols, (const unsigned long long*)pvData);
                }
            }
        }


        return 0;
    }

    static int convertContextToStackBool(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos)
    {
        Bool* pB = _pIT->getAs<Bool>();
        int iRows = pB->getRows();
        int iCols = pB->getCols();
        int* piBool = pB->get();

        if(_piAddress)
        {
            stackapi_createMatrixOfBooleanInList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iRows, iCols, piBool);
        }
        else
        {
            stackapi_createMatrixOfBoolean(stackapi_pvApiCtx, _iPos, iRows, iCols, piBool);
        }

        return 0;
    }

    static int convertContextToStackList(InternalType* _pIT, int* _piAddress, int _iIndex, int _iPos)
    {
        List* pL = _pIT->getAs<List>();
        int iItem = pL->getSize();
        int* piRoot = 0;

        if(pL->isMList())
        {
            if(_piAddress)
            {
                stackapi_createMListInList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iItem, &piRoot);
            }
            else
            {
                stackapi_createMList(stackapi_pvApiCtx, _iPos, iItem, &piRoot);
            }
        }
        else if(pL->isTList())
        {
            if(_piAddress)
            {
                stackapi_createTListInList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iItem, &piRoot);
            }
            else
            {
                stackapi_createTList(stackapi_pvApiCtx, _iPos, iItem, &piRoot);
            }
        }
        else
        {
            if(_piAddress)
            {
                stackapi_createListInList(stackapi_pvApiCtx, _iPos, _piAddress, _iIndex, iItem, &piRoot);
            }
            else
            {
                stackapi_createList(stackapi_pvApiCtx, _iPos, iItem, &piRoot);
            }
        }

        if(piRoot == NULL)
        {
            return 1;
        }

        for(int i = 0 ; i < iItem ; i++)
        {
            InternalType* pITChild = pL->get(i);
            if(convertContextToStackData(pITChild, piRoot, i + 1, _iPos) != 0)
            {
                return 1;
            }
        }

        return 0;
    }

    /*stack to Context*/
    InternalType* convertStackToContext(int _iPos)
    {
        int* piAddr = NULL;
        stackapi_getVarAddressFromPosition(stackapi_pvApiCtx, _iPos, &piAddr);

        return convertStackToContextData(piAddr);
    }


    static InternalType* convertStackToContextData(int* _piAddr)
    {
        int iType = 0;
        InternalType* pIT = NULL;

        stackapi_getVarType(stackapi_pvApiCtx, _piAddr, &iType);

        switch (iType)
        {
        case sci_matrix :
            {
                pIT = convertStackToContextDouble(_piAddr);
                break;
            }

        case sci_strings :
            {
                pIT = convertStackToContextString(_piAddr);
                break;
            }

        case sci_poly :
            {
                pIT = convertStackToContextPoly(_piAddr);
                break;
            }

        case sci_list :
        case sci_tlist :
        case sci_mlist :
            {
                pIT = convertStackToContextList(_piAddr);
                break;
            }

        default :
            {
                printf("convertStackToContextData Unmanaged type %d\n", iType);
                return Double::Empty(); //FOR DEBUG ONLY
            }
        }
        return pIT;
    }

    static InternalType* convertStackToContextList(int* _piAddr)
    {
        int iNbItem = 0;

        stackapi_getListItemNumber(stackapi_pvApiCtx, _piAddr, &iNbItem);

        List* pL = new List();
        for(int i = 0 ; i < iNbItem ; i++)
        {
            int* piItemAddr = NULL;
            stackapi_getListItemAddress(stackapi_pvApiCtx, _piAddr, i + 1, &piItemAddr);
            pL->append(convertStackToContextData(piItemAddr));
        }

        return pL;
    }

    static InternalType* convertStackToContextDouble(int* _piAddr)
    {
        InternalType* pIT = NULL;
        int iRows = 0;
        int iCols = 0;
        double* pdblReal = NULL;
        double* pdblImg = NULL;

        /*get data from stack*/
        stackapi_getComplexMatrixOfDouble(stackapi_pvApiCtx, _piAddr, &iRows, &iCols, &pdblReal, &pdblImg);

        Double* pDbl = new Double(iRows, iCols, pdblImg != NULL);
        pDbl->set(pdblReal);

        /*create new variable*/
        if (pdblImg)
        {
            pDbl->setImg(pdblImg);
        }

        /*return new variable*/
        return pDbl;
    }

    static InternalType* convertStackToContextString(int* _piAddr)
    {
        int iRows = 0;
        int iCols = 0;
        char** pstData = NULL;

        /*get data from stack*/
        stackapi_getAllocatedMatrixOfString(stackapi_pvApiCtx, _piAddr, &iRows, &iCols, &pstData);
        String* pS = new String(iRows, iCols, pstData);

        /*release temporary data*/
        freeArrayOfString(pstData, iRows * iCols);
        /*return new variable*/
        return pS;
    }

    static InternalType* convertStackToContextPoly(int* _piAddr)
    {
        int iRows = 0;
        int iCols = 0;
        int *piRank = NULL;
        double** pdblReal = NULL;
        double** pdblImg = NULL;
        char* pstVarname = (char*)MALLOC(sizeof(char) * 5);
        int iLen = 0;

        stackapi_getPolyVariableName(stackapi_pvApiCtx, _piAddr, pstVarname, &iLen);
        stackapi_getAllocatedMatrixOfComplexPoly(stackapi_pvApiCtx, _piAddr, &iRows, &iCols, &piRank, &pdblReal, &pdblImg);

        wchar_t* pwstVarname = to_wide_string(pstVarname);

        Polynom* pP = new Polynom(pwstVarname, iRows, iCols, piRank);
        FREE(pstVarname);
        FREE(pwstVarname);
        if (pdblImg)
        {
            pP->setComplex(true);
        }

        int iOffset = 0;
        for (int i = 0 ; i < iRows * iCols ; i++)
        {
            Double* pCoef = new Double(1, piRank[i], pP->isComplex());
            pCoef->set(pdblReal[i]);
            if (pP->isComplex())
            {
                pCoef->setImg(pdblImg[i]);
            }

            pP->setCoef(i, pCoef);
        }

        delete piRank;
        return pP;
    }

    static InternalType* convertStackToContextInt(int* _piAddr)
    {
        InternalType* pIT = NULL;


        return pIT;
    }

    static InternalType* convertStackToContextBool(int* _piAddr)
    {
        int iRows = 0;
        int iCols = 0;
        int* piBool = NULL;

        stackapi_getMatrixOfBoolean(stackapi_pvApiCtx, _piAddr, &iRows, &iCols, &piBool);

        Bool* pB = new Bool(iRows, iCols);
        pB->set(piBool);
        return pB;
    }
} //namespace

