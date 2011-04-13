/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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
#include "funcmanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "scilab_sscanf.hxx"
#include "checkformat.hxx"
#include "yaspio.hxx"
#include "cell.hxx"
#include "int.hxx"

extern "C"
{
#include <stdio.h>
#include "localization.h"
#include "Scierror.h"
}


types::Function::ReturnValue sci_mscanf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int size                    = in.size();
    int iNiter                  = 1;
    int iNumberPercent          = 0;        // iNumberPercent = all % - %* - %n
    int iCountDataRead          = 0;
    wchar_t* wcsFormat          = NULL;
    wchar_t* wcsRead            = NULL;
    unsigned int uiFormat[1]    = {0};
    int dimsArray[2]            = {1,1};
    std::vector<types::InternalType*>* pIT = new std::vector<types::InternalType*>();

    if(size < 1 || size > 2)
    {
       ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"mscanf", 1, 2);
        return types::Function::Error;
    }

    if(size == 2)
    {
        if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A Real expected.\n"), L"mscanf", 1);
            return types::Function::Error;
        }
        iNiter = static_cast<int>(in[0]->getAs<types::Double>()->getReal()[0]);
    }

    if(in[size-1]->isString() == false || in[size-1]->getAs<types::String>()->isScalar() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"mscanf", size);
        return types::Function::Error;
    }

    wcsFormat = in[size-1]->getAs<types::String>()->get(0);

// check format
    iNumberPercent = checkFormat(wcsFormat, uiFormat);

    if(iNumberPercent < 0)
    {
        if(iNumberPercent == CHECK_ERROR_BAD_FORMAT)
        {
            ScierrorW(999, _W("%ls: Wrong format for input argument #%d: Bad format after %% char.\n"), L"mscanf", size);
            return types::Function::Error;
        }
        if(iNumberPercent == CHECK_ERROR_INCOMPLETE_FORMAT)
        {
            ScierrorW(999, _W("%ls: Wrong format for input argument #%d: Incomplete format.\n"), L"mscanf", size);
            return types::Function::Error;
        }
    }

    for(int i = 0; i < iNiter; i++)
    {
        // get data
        wcsRead = to_wide_string(YaspRead());
        // perform scanf
        iCountDataRead = scilab_sscanf(wcsFormat, wcsRead, i, iNiter, pIT);
        FREE(wcsRead);
        if(iCountDataRead < -1)
        {
            ScierrorW(999, _W("%ls: Wrong input data in argument #%d.\n"), L"mscanf", size);
            return types::Function::Error;
        }
    }

    int sizeOfVector = pIT->size();
    if(sizeOfVector == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if(_iRetCount > 1)
    {
        types::Double* pDouble = new types::Double(2, dimsArray);
        pDouble->set(0, iCountDataRead);
        out.push_back(pDouble);

        for(int i = 0; i < sizeOfVector; i++)
        {
            out.push_back((*pIT)[i]);
        }
        for(int i = sizeOfVector + 1; i < _iRetCount; i++)
        {
            out.push_back(types::Double::Empty());
        }
    }
    else
    {
        if(sizeOfVector != iCountDataRead)
        {
            *uiFormat = 0;
        }
        switch(*uiFormat) // If the format is the same (ex : "%d%d%d"), return a matrix of this format (ex : types::Int32)  else retrun a Cell.
        {
            case    1     | (1 << 10): // %c
            case (1 << 6) | (1 << 10): // %s %[]
            case (1 << 6) | (1 << 11): // %ls
            {
                int sizeOfString = (*pIT)[0]->getAs<types::String>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfString, sizeOfVector};
                types::String* pString = new types::String(2, dimsArrayOfRes);
                for(int i = 0; i < sizeOfVector; i++)
                {
                    for(int j = 0; j < sizeOfString; j++)
                    {
                        pString->set(i*sizeOfString+j, (*pIT)[i]->getAs<types::String>()->get(j));
                    }
                }
                out.push_back(pString);
            }
            break;
            case (1 << 8) | (1 << 10): // %u
            case (1 << 8) | (1 << 9) : // %hu
            case (1 << 8) | (1 << 11): // %lu %Lu
            case (1 << 7) | (1 << 10): // %x %X
            case (1 << 7) | (1 << 9) : // %hx %hX
            case (1 << 7) | (1 << 11): // %lx %lX
            case (1 << 1) | (1 << 10): // %d %i
            case (1 << 1) | (1 << 9) : // %hd
            case (1 << 1) | (1 << 11): // %ld %Ld
            case (1 << 5) | (1 << 10): // %o
            case (1 << 5) | (1 << 9) : // %ho
            case (1 << 5) | (1 << 11): // %lo
            {
                switch((*pIT)[0]->getType())
                {
                    case types::InternalType::RealInt16 :
                    {
                        int sizeOfInt = (*pIT)[0]->getAs<types::Int16>()->getRows();
                        int dimsArrayOfRes[2] = {sizeOfInt, sizeOfVector};
                        types::Int16* pInt = new types::Int16(2, dimsArrayOfRes);
                        for(int i = 0; i < sizeOfVector; i++)
                        {
                            for(int j = 0; j < sizeOfInt; j++)
                            {
                                pInt->set(i*sizeOfInt+j, (*pIT)[i]->getAs<types::Int16>()->get(j));
                            }
                        }
                        out.push_back(pInt);
                    }
                    break;
                    case types::InternalType::RealInt32 :
                    {
                        int sizeOfInt = (*pIT)[0]->getAs<types::Int32>()->getRows();
                        int dimsArrayOfRes[2] = {sizeOfInt, sizeOfVector};
                        types::Int32* pInt = new types::Int32(2, dimsArrayOfRes);

                        for(int i = 0; i < sizeOfVector; i++)
                        {
                            for(int j = 0; j < sizeOfInt; j++)
                            {
                                pInt->set(i*sizeOfInt+j, (*pIT)[i]->getAs<types::Int32>()->get(j));
                            }
                        }
                        out.push_back(pInt);
                    }
                    break;
                    case types::InternalType::RealInt64 :
                    {
                        int sizeOfInt = (*pIT)[0]->getAs<types::Int64>()->getRows();
                        int dimsArrayOfRes[2] = {sizeOfInt, sizeOfVector};
                        types::Int64* pInt = new types::Int64(2, dimsArrayOfRes);

                        for(int i = 0; i < sizeOfVector; i++)
                        {
                            for(int j = 0; j < sizeOfInt; j++)
                            {
                                pInt->set(i*sizeOfInt+j, (*pIT)[i]->getAs<types::Int64>()->get(j));
                            }
                        }
                        out.push_back(pInt);
                    }
                    break;
                    case types::InternalType::RealUInt16 :
                    {
                        int sizeOfInt = (*pIT)[0]->getAs<types::UInt16>()->getRows();
                        int dimsArrayOfRes[2] = {sizeOfInt, sizeOfVector};
                        types::UInt16* pInt = new types::UInt16(2, dimsArrayOfRes);
                        for(int i = 0; i < sizeOfVector; i++)
                        {
                            for(int j = 0; j < sizeOfInt; j++)
                            {
                                pInt->set(i*sizeOfInt+j, (*pIT)[i]->getAs<types::UInt16>()->get(j));
                            }
                        }
                        out.push_back(pInt);
                    }
                    break;
                    case types::InternalType::RealUInt32 :
                    {
                        int sizeOfInt = (*pIT)[0]->getAs<types::UInt32>()->getRows();
                        int dimsArrayOfRes[2] = {sizeOfInt, sizeOfVector};
                        types::UInt32* pInt = new types::UInt32(2, dimsArrayOfRes);

                        for(int i = 0; i < sizeOfVector; i++)
                        {
                            for(int j = 0; j < sizeOfInt; j++)
                            {
                                pInt->set(i*sizeOfInt+j, (*pIT)[i]->getAs<types::UInt32>()->get(j));
                            }
                        }
                        out.push_back(pInt);
                    }
                    break;
                    case types::InternalType::RealUInt64 :
                    {
                        int sizeOfInt = (*pIT)[0]->getAs<types::UInt64>()->getRows();
                        int dimsArrayOfRes[2] = {sizeOfInt, sizeOfVector};
                        types::UInt64* pInt = new types::UInt64(2, dimsArrayOfRes);

                        for(int i = 0; i < sizeOfVector; i++)
                        {
                            for(int j = 0; j < sizeOfInt; j++)
                            {
                                pInt->set(i*sizeOfInt+j, (*pIT)[i]->getAs<types::UInt64>()->get(j));
                            }
                        }
                        out.push_back(pInt);
                    }
                    break;
                    default :
                        return types::Function::Error;
                }
            }
            break;
            case (1 << 2) | (1 << 10): // %e %E
            case (1 << 2) | (1 << 11): // long
            case (1 << 3) | (1 << 10): // %g %G
            case (1 << 3) | (1 << 11): // long
            case (1 << 4) | (1 << 10): // %f => float
            case (1 << 4) | (1 << 11): // %lf => double
            {
                int sizeOfDouble = (*pIT)[0]->getAs<types::Double>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfDouble, sizeOfVector};
                types::Double* pDouble = new types::Double(2, dimsArrayOfRes);
                for(int i = 0; i < sizeOfVector; i++)
                {
                    for(int j = 0; j < sizeOfDouble; j++)
                    {
                        pDouble->set(i*sizeOfDouble+j, (*pIT)[i]->getAs<types::Double>()->get(j));
                    }
                }
                out.push_back(pDouble);
            }
            default :
            {
                std::vector<types::InternalType*>* pITTemp = new std::vector<types::InternalType*>();
                pITTemp->push_back((*pIT)[0]);

                // sizeOfVector always > 1
                for(int i = 1; i < sizeOfVector; i++) // concatenates the Cells. ex : [String 4x1] [String 4x1] = [String 4x2]
                {
                    if(pITTemp->back()->getType() == (*pIT)[i]->getType())
                    {
                        switch(pITTemp->back()->getType())
                        {
                            case types::InternalType::RealString :
                            {
                                int iRows               = pITTemp->back()->getAs<types::String>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::String>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::String* pType    = new types::String(2, arrayOfType);

                                for(int k=0; k<pITTemp->back()->getAs<types::String>()->getSize(); k++)
                                {
                                    pType->set(k, pITTemp->back()->getAs<types::String>()->get(k));
                                }
                                for(int k=0; k<(*pIT)[i]->getAs<types::String>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::String>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            case types::InternalType::RealDouble :
                            {
                                int iRows               = pITTemp->back()->getAs<types::Double>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::Double>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::Double* pType    = new types::Double(2, arrayOfType);

                                pType->set(pITTemp->back()->getAs<types::Double>()->get());
                                for(int k=0; k<(*pIT)[i]->getAs<types::Double>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::Double>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            case types::InternalType::RealInt16 :
                            {
                                int iRows               = pITTemp->back()->getAs<types::Int16>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::Int16>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::Int16* pType    = new types::Int16(2, arrayOfType);

                                pType->set(pITTemp->back()->getAs<types::Int16>()->get());
                                for(int k=0; k<(*pIT)[i]->getAs<types::Int16>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::Int16>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            case types::InternalType::RealInt32 :
                            {
                                int iRows               = pITTemp->back()->getAs<types::Int32>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::Int32>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::Int32* pType    = new types::Int32(2, arrayOfType);

                                pType->set(pITTemp->back()->getAs<types::Int32>()->get());
                                for(int k=0; k<(*pIT)[i]->getAs<types::Int32>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::Int32>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            case types::InternalType::RealInt64 :
                            {
                                int iRows               = pITTemp->back()->getAs<types::Int64>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::Int64>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::Int64* pType    = new types::Int64(2, arrayOfType);

                                pType->set(pITTemp->back()->getAs<types::Int64>()->get());
                                for(int k=0; k<(*pIT)[i]->getAs<types::Int64>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::Int64>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            case types::InternalType::RealUInt16 :
                            {
                                int iRows               = pITTemp->back()->getAs<types::UInt16>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::UInt16>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::UInt16* pType    = new types::UInt16(2, arrayOfType);

                                pType->set(pITTemp->back()->getAs<types::UInt16>()->get());
                                for(int k=0; k<(*pIT)[i]->getAs<types::UInt16>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::UInt16>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            case types::InternalType::RealUInt32 :
                            {
                                int iRows               = pITTemp->back()->getAs<types::UInt32>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::UInt32>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::UInt32* pType    = new types::UInt32(2, arrayOfType);

                                pType->set(pITTemp->back()->getAs<types::UInt32>()->get());
                                for(int k=0; k<(*pIT)[i]->getAs<types::UInt32>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::UInt32>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            case types::InternalType::RealUInt64 :
                            {
                                int iRows               = pITTemp->back()->getAs<types::UInt64>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::UInt64>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::UInt64* pType    = new types::UInt64(2, arrayOfType);

                                pType->set(pITTemp->back()->getAs<types::UInt64>()->get());
                                for(int k=0; k<(*pIT)[i]->getAs<types::UInt64>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::UInt64>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            default :
                                return types::Function::Error;
                        }
                    }
                    else
                    {
                        pITTemp->push_back((*pIT)[i]);
                    }
                }

                int dimsArrayOfCell[2] = {1, pITTemp->size()};
                types::Cell* pCell = new types::Cell(2, dimsArrayOfCell);
                for(int i = 0; i < pITTemp->size(); i++)
                {
                    pCell->set(i, (*pITTemp)[i]);
                }
                out.push_back(pCell);
            }
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
