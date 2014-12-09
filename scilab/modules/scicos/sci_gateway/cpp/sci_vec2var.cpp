/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string>
#include <cwchar>
#include <cstring>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "function.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

const std::string funname = "vec2var";

static int fill_double(const double* const tab, const int iDims, types::Double* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    if (iElements == 0)
    {
        res = types::Double::Empty();
        delete[] pDims;
        return 5;
    }
    bool isComplex = (tab[iDims] == 1);

    res = new types::Double(iDims, pDims, isComplex);
    delete[] pDims;

    res->set(tab + iDims + 1);
    if (isComplex == true)
    {
        res->setImg(tab + iDims + 1 + iElements);
    }

    return 2 + iDims + 1 + iElements;
}

static int fill_int8(const double* const tab, const int iDims, types::Int8* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    int numberOfIntNeeded = ((iElements - 1) / 8) + 1;

    res = new types::Int8(iDims, pDims);
    delete[] pDims;

    //Use a buffer to prevent copying only parts of integers
    char* buffer = new char[numberOfIntNeeded * 8];
    memcpy(buffer, tab + iDims, numberOfIntNeeded * sizeof(double));
    memcpy(res->get(), buffer, iElements * sizeof(char));
    delete[] buffer;

    return 2 + iDims + numberOfIntNeeded;
}

static int fill_uint8(const double* const tab, const int iDims, types::UInt8* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    int numberOfIntNeeded = ((iElements - 1) / 8) + 1;

    res = new types::UInt8(iDims, pDims);
    delete[] pDims;

    //Use a buffer to prevent copying only parts of integers
    unsigned char* buffer = new unsigned char[numberOfIntNeeded * 8];
    memcpy(buffer, tab + iDims, numberOfIntNeeded * sizeof(double));
    memcpy(res->get(), buffer, iElements * sizeof(unsigned char));
    delete[] buffer;

    return 2 + iDims + numberOfIntNeeded;
}

static int fill_int16(const double* const tab, const int iDims, types::Int16* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    int numberOfIntNeeded = ((iElements - 1) / 4) + 1;

    res = new types::Int16(iDims, pDims);
    delete[] pDims;

    //Use a buffer to prevent copying only parts of integers
    short int* buffer = new short int[numberOfIntNeeded * 4];
    memcpy(buffer, tab + iDims, numberOfIntNeeded * sizeof(double));
    memcpy(res->get(), buffer, iElements * sizeof(short int));
    delete[] buffer;

    return 2 + iDims + numberOfIntNeeded;
}

static int fill_uint16(const double* const tab, const int iDims, types::UInt16* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    int numberOfIntNeeded = ((iElements - 1) / 4) + 1;

    res = new types::UInt16(iDims, pDims);
    delete[] pDims;

    //Use a buffer to prevent copying only parts of integers
    unsigned short int* buffer = new unsigned short int[numberOfIntNeeded * 4];
    memcpy(buffer, tab + iDims, numberOfIntNeeded * sizeof(double));
    memcpy(res->get(), buffer, iElements * sizeof(unsigned short int));
    delete[] buffer;

    return 2 + iDims + numberOfIntNeeded;
}

static int fill_int32(const double* const tab, const int iDims, types::Int32* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    int numberOfIntNeeded = ((iElements - 1) / 2) + 1;

    res = new types::Int32(iDims, pDims);
    delete[] pDims;

    //Use a buffer to prevent copying only parts of integers
    int* buffer = new int[numberOfIntNeeded * 2];
    memcpy(buffer, tab + iDims, numberOfIntNeeded * sizeof(double));
    memcpy(res->get(), buffer, iElements * sizeof(int));
    delete[] buffer;

    return 2 + iDims + numberOfIntNeeded;
}

static int fill_uint32(const double* const tab, const int iDims, types::UInt32* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    int numberOfIntNeeded = ((iElements - 1) / 2) + 1;

    res = new types::UInt32(iDims, pDims);
    delete[] pDims;

    //Use a buffer to prevent copying only parts of integers
    unsigned int* buffer = new unsigned int[numberOfIntNeeded * 2];
    memcpy(buffer, tab + iDims, numberOfIntNeeded * sizeof(double));
    memcpy(res->get(), buffer, iElements * sizeof(unsigned int));
    delete[] buffer;

    return 2 + iDims + numberOfIntNeeded;
}

static int fill_bool(const double* const tab, const int iDims, types::Bool* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }

    res = new types::Bool(iDims, pDims);
    delete[] pDims;

    for (int i = 0; i < iElements; ++i)
    {
        res->set(i, tab[iDims + i]);
    }

    return 2 + iDims + iElements;
}

static int fill_string(const double* const tab, const int iDims, types::String* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }

    res = new types::String(iDims, pDims);
    delete[] pDims;

    int offset = 0;
    for (int i = 0; i < iElements; ++i)
    {
        int strLen = tab[iDims + offset];

        wchar_t* str = new wchar_t[strLen + 1];
        for (int j = 0; j < strLen; ++j)
        {
            str[j] = tab[iDims + offset + 1 + j];
        }
        str[strLen] = '\0';
        res->set(i, str);
        delete[] str;

        offset += 1 + strLen;
    }

    return 2 + iDims + offset;
}

types::Function::ReturnValue sci_vec2var(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }

    if (!in[0]->isDouble())
    {
        Scierror(999, _("%s:  Wrong type for input argument #%d: A real row vector expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }
    types::Double* input = in[0]->getAs<types::Double>();

    if (input->getCols() != 1)
    {
        Scierror(999, _("%s:  Wrong size for input argument #%d: A row vector expected.\n"), funname.c_str(), 1);
        return types::Function::Error;
    }
    if (input->getRows() < 2)
    {
        Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 2, 1);
        return types::Function::Error;
    }

    int iType = input->get(0);
    int iDims = input->get(1);

    switch (iType)
    {
        case types::InternalType::ScilabDouble :
        {
            if (input->getRows() < 5)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 5, 1);
                return types::Function::Error;
            }

            types::Double* pDouble;
            fill_double(input->get() + 2, iDims, pDouble);

            out.push_back(pDouble);
            break;
        }

        case types::InternalType::ScilabInt8   :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::Int8* pInt8;
            fill_int8(input->get() + 2, iDims, pInt8);

            out.push_back(pInt8);
            break;
        }

        case types::InternalType::ScilabUInt8  :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::UInt8* pUInt8;
            fill_uint8(input->get() + 2, iDims, pUInt8);

            out.push_back(pUInt8);
            break;
        }

        case types::InternalType::ScilabInt16  :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::Int16* pInt16;
            fill_int16(input->get() + 2, iDims, pInt16);

            out.push_back(pInt16);
            break;
        }

        case types::InternalType::ScilabUInt16 :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::UInt16* pUInt16;
            fill_uint16(input->get() + 2, iDims, pUInt16);

            out.push_back(pUInt16);
            break;
        }

        case types::InternalType::ScilabInt32  :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::Int32* pInt32;
            fill_int32(input->get() + 2, iDims, pInt32);

            out.push_back(pInt32);
            break;
        }

        case types::InternalType::ScilabUInt32 :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::UInt32* pUInt32;
            fill_uint32(input->get() + 2, iDims, pUInt32);

            out.push_back(pUInt32);
            break;
        }

        case types::InternalType::ScilabBool   :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::Bool* pBool;
            fill_bool(input->get() + 2, iDims, pBool);

            out.push_back(pBool);
            break;
        }

        case types::InternalType::ScilabString :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::String* pString;
            fill_string(input->get() + 2, iDims, pString);

            out.push_back(pString);
            break;
        }

        case types::InternalType::ScilabList   :
        {
            types::List* pList = new types::List();

            int offset = 0;
            for (int i = 0; i < iDims; ++i)
            {
                switch (static_cast<int>(input->get(2 + offset)))
                {
                    case types::InternalType::ScilabDouble :
                    {
                        types::Double* pDouble;
                        offset += fill_double(input->get() + 4 + offset, input->get(3 + offset), pDouble);
                        pList->append(pDouble);
                        break;
                    }
                    case types::InternalType::ScilabInt8   :
                    {
                        types::Int8* pInt8;
                        offset += fill_int8(input->get() + 4 + offset, input->get(3 + offset), pInt8);
                        pList->append(pInt8);
                        break;
                    }
                    case types::InternalType::ScilabUInt8  :
                    {
                        types::UInt8* pUInt8;
                        offset += fill_uint8(input->get() + 4 + offset, input->get(3 + offset), pUInt8);
                        pList->append(pUInt8);
                        break;
                    }
                    case types::InternalType::ScilabInt16  :
                    {
                        types::Int16* pInt16;
                        offset += fill_int16(input->get() + 4 + offset, input->get(3 + offset), pInt16);
                        pList->append(pInt16);
                        break;
                    }
                    case types::InternalType::ScilabUInt16 :
                    {
                        types::UInt16* pUInt16;
                        offset += fill_uint16(input->get() + 4 + offset, input->get(3 + offset), pUInt16);
                        pList->append(pUInt16);
                        break;
                    }
                    case types::InternalType::ScilabInt32  :
                    {
                        types::Int32* pInt32;
                        offset += fill_int32(input->get() + 4 + offset, input->get(3 + offset), pInt32);
                        pList->append(pInt32);
                        break;
                    }
                    case types::InternalType::ScilabUInt32 :
                    {
                        types::UInt32* pUInt32;
                        offset += fill_uint32(input->get() + 4 + offset, input->get(3 + offset), pUInt32);
                        pList->append(pUInt32);
                        break;
                    }
                    case types::InternalType::ScilabBool   :
                    {
                        types::Bool* pBool;
                        offset += fill_bool(input->get() + 4 + offset, input->get(3 + offset), pBool);
                        pList->append(pBool);
                        break;
                    }
                    case types::InternalType::ScilabString :
                    {
                        types::String* pString;
                        offset += fill_string(input->get() + 4 + offset, input->get(3 + offset), pString);
                        pList->append(pString);
                        break;
                    }
                    default :
                    {
                        Scierror(999, _("%s: Wrong value for element #%d of input argument #%d: Unknown type.\n"), funname.c_str(), 3 + offset, 1);
                        return types::Function::Error;
                    }
                }
            }

            out.push_back(pList);
            break;
        }

        default :
        {
            Scierror(999, _("%s: Wrong value for element #%d of input argument #%d: Unknown type.\n"), funname.c_str(), 1, 1);
            return types::Function::Error;
        }
    }

    return types::Function::OK;
}
