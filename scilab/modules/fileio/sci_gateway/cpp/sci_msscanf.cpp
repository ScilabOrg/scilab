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


types::Function::ReturnValue sci_msscanf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int size                    = in.size();
    int iNiter                  = 1;
    int iNumberPercent          = 0;
    wchar_t* wcsFormat          = NULL;
    types::String* pStrRead     = NULL;
    int dimsArray[2]            = {1,1};
    unsigned int uiFormat[1]    = {0};

    std::vector<types::InternalType*>* pIT = new std::vector<types::InternalType*>();

    if(size < 2 || size >3)
    {
       ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"msscanf", 2, 3);
        return types::Function::Error;
    }

    if(size == 3)
    {
        if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A Real expected.\n"), L"msscanf", 1);
            return types::Function::Error;
        }
        iNiter = static_cast<int>(in[0]->getAs<types::Double>()->getReal()[0]);
    }

    if(in[size-2]->isString() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A Strings expected.\n"), L"msscanf", size-1);
        return types::Function::Error;
    }

    if(in[size-1]->isString() == false || in[size-1]->getAs<types::String>()->isScalar() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"msscanf", size);
        return types::Function::Error;
    }

    pStrRead  = in[size-2]->getAs<types::String>();
    if(iNiter == -1)
    {
        iNiter = pStrRead->getRows();
    }
    wcsFormat = in[size-1]->getAs<types::String>()->get(0);

// check format
    iNumberPercent = checkFormat(wcsFormat, uiFormat);
    if(iNumberPercent < 0)
    {
        if(iNumberPercent == CHECK_ERROR_BAD_FORMAT)
        {
            ScierrorW(999, _W("%ls: Wrong format for input argument #%d: Bad format after %% char.\n"), L"msscanf", size);
            return types::Function::Error;
        }
        if(iNumberPercent == CHECK_ERROR_INCOMPLETE_FORMAT)
        {
            ScierrorW(999, _W("%ls: Wrong format for input argument #%d: Incomplete format.\n"), L"msscanf", size);
            return types::Function::Error;
        }
    }

// get data
    if(pStrRead->getSize() < iNiter)
    {
        ScierrorW(999,_W("%ls: An error occurred: %ls\n"),L"msscanf",_W("Not enough entries."));
        return types::Function::Error;
    }

    for(int i = 0; i < iNiter; i++)
    {
        // perform scanf
        int error = scilab_sscanf(wcsFormat, pStrRead->get(i), i, iNiter, pIT);
        if(error)
        {
            ScierrorW(999, _W("%ls: Wrong input data in argument #%d.\n"), L"msscanf", size);
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
        if(sizeOfVector + 1 != _iRetCount)
        {
            ScierrorW(77, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"msscanf", 1, sizeOfVector+1);
            return types::Function::Error;
        }
        types::Double* pDouble = new types::Double(2,dimsArray);
        pDouble->set(0,iNumberPercent);
        out.push_back(pDouble);

        for(int i = 0; i < sizeOfVector; i++)
        {
            out.push_back((*pIT)[i]);
        }
    }
    else
    {
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
                int dimsArrayOfCell[2] = {1, sizeOfVector};
                types::Cell* pCell = new types::Cell(2, dimsArrayOfCell);
                for(int i = 0; i < sizeOfVector; i++)
                {
                    pCell->set(i, (*pIT)[i]);
                }
                out.push_back(pCell);
            }
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
