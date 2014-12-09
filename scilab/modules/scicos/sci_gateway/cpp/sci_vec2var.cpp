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
    if (input->getRows() < 4)
    {
        Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 4, 1);
        return types::Function::Error;
    }

    int iType = input->get(0);
    int iDims = input->get(1);
    int iElements = 1;

    switch (iType)
    {
        case types::InternalType::ScilabDouble :
        {
            int* pDims = new int[iDims];
            for (int i = 0; i < iDims; ++i)
            {
                pDims[i] = input->get(2 + i);
                iElements *= pDims[i];
            }
            if (pDims[0] == 0)
            {
                out.push_back(types::Double::Empty());
                break;
            }
            bool isComplex = (input->get(2 + iDims) == 1);

            types::Double* pDouble = new types::Double(iDims, pDims, isComplex);
            delete[] pDims;

            pDouble->set(input->get() + 3 + iDims);
            if (isComplex == true)
            {
                pDouble->setImg(input->get() + 3 + iDims + iElements);
            }

            out.push_back(pDouble);
            break;
        }

        case types::InternalType::ScilabInt8   :
        {
            int* pDims = new int[iDims];
            for (int i = 0; i < iDims; ++i)
            {
                pDims[i] = input->get(2 + i);
                iElements *= pDims[i];
            }
            int numberOfIntNeeded = ((iElements - 1) / 8) + 1;

            types::Int8* pInt8 = new types::Int8(iDims, pDims);
            delete[] pDims;

            // Use a buffer to prevent copying only parts of integers
            char* buffer = new char[numberOfIntNeeded * 8];
            memcpy(buffer, input->get() + 2 + iDims, numberOfIntNeeded * sizeof(double));
            memcpy(pInt8->get(), buffer, iElements * sizeof(char));
            delete[] buffer;

            out.push_back(pInt8);
            break;
        }

        case types::InternalType::ScilabUInt8  :
        {
            int* pDims = new int[iDims];
            for (int i = 0; i < iDims; ++i)
            {
                pDims[i] = input->get(2 + i);
                iElements *= pDims[i];
            }
            int numberOfIntNeeded = ((iElements - 1) / 8) + 1;

            types::UInt8* pUInt8 = new types::UInt8(iDims, pDims);
            delete[] pDims;

            // Use a buffer to prevent copying only parts of integers
            unsigned char* buffer = new unsigned char[numberOfIntNeeded * 8];
            memcpy(buffer, input->get() + 2 + iDims, numberOfIntNeeded * sizeof(double));
            memcpy(pUInt8->get(), buffer, iElements * sizeof(unsigned char));
            delete[] buffer;

            out.push_back(pUInt8);
            break;
        }

        case types::InternalType::ScilabInt16  :
        {
            int* pDims = new int[iDims];
            for (int i = 0; i < iDims; ++i)
            {
                pDims[i] = input->get(2 + i);
                iElements *= pDims[i];
            }
            int numberOfIntNeeded = ((iElements - 1) / 4) + 1;

            types::Int16* pInt16 = new types::Int16(iDims, pDims);
            delete[] pDims;

            // Use a buffer to prevent copying only parts of integers
            short int* buffer = new short int[numberOfIntNeeded * 4];
            memcpy(buffer, input->get() + 2 + iDims, numberOfIntNeeded * sizeof(double));
            memcpy(pInt16->get(), buffer, iElements * sizeof(short int));
            delete[] buffer;

            out.push_back(pInt16);
            break;
        }

        case types::InternalType::ScilabUInt16 :
        {
            int* pDims = new int[iDims];
            for (int i = 0; i < iDims; ++i)
            {
                pDims[i] = input->get(2 + i);
                iElements *= pDims[i];
            }
            int numberOfIntNeeded = ((iElements - 1) / 4) + 1;

            types::UInt16* pUInt16 = new types::UInt16(iDims, pDims);
            delete[] pDims;

            // Use a buffer to prevent copying only parts of integers
            unsigned short int* buffer = new unsigned short int[numberOfIntNeeded * 4];
            memcpy(buffer, input->get() + 2 + iDims, numberOfIntNeeded * sizeof(double));
            memcpy(pUInt16->get(), buffer, iElements * sizeof(unsigned short int));
            delete[] buffer;

            out.push_back(pUInt16);
            break;
        }

        case types::InternalType::ScilabInt32  :
        {
            int* pDims = new int[iDims];
            for (int i = 0; i < iDims; ++i)
            {
                pDims[i] = input->get(2 + i);
                iElements *= pDims[i];
            }
            int numberOfIntNeeded = ((iElements - 1) / 2) + 1;

            types::Int32* pInt32 = new types::Int32(iDims, pDims);
            delete[] pDims;

            // Use a buffer to prevent copying only parts of integers
            int* buffer = new int[numberOfIntNeeded * 2];
            memcpy(buffer, input->get() + 2 + iDims, numberOfIntNeeded * sizeof(double));
            memcpy(pInt32->get(), buffer, iElements * sizeof(int));
            delete[] buffer;

            out.push_back(pInt32);
            break;
        }

        case types::InternalType::ScilabUInt32 :
        {
            int* pDims = new int[iDims];
            for (int i = 0; i < iDims; ++i)
            {
                pDims[i] = input->get(2 + i);
                iElements *= pDims[i];
            }
            int numberOfIntNeeded = ((iElements - 1) / 2) + 1;

            types::UInt32* pUInt32 = new types::UInt32(iDims, pDims);
            delete[] pDims;

            // Use a buffer to prevent copying only parts of integers
            unsigned int* buffer = new unsigned int[numberOfIntNeeded * 2];
            memcpy(buffer, input->get() + 2 + iDims, numberOfIntNeeded * sizeof(double));
            memcpy(pUInt32->get(), buffer, iElements * sizeof(unsigned int));
            delete[] buffer;

            out.push_back(pUInt32);
            break;
        }

        case types::InternalType::ScilabBool   :
        {
            int* pDims = new int[iDims];
            for (int i = 0; i < iDims; ++i)
            {
                pDims[i] = input->get(2 + i);
                iElements *= pDims[i];
            }

            types::Bool* pBool = new types::Bool(iDims, pDims);
            delete[] pDims;

            for (int i = 0; i < iElements; ++i)
            {
                pBool->set(i, input->get(2 + iDims + i));
            }

            out.push_back(pBool);
            break;
        }

        case types::InternalType::ScilabString :
        {
            int* pDims = new int[iDims];
            for (int i = 0; i < iDims; ++i)
            {
                pDims[i] = input->get(2 + i);
                iElements *= pDims[i];
            }

            types::String* pString = new types::String(iDims, pDims);
            delete[] pDims;

            int offset = 0;
            for (int i = 0; i < iElements; ++i)
            {
                int strLen = input->get(2 + iDims + offset);

                wchar_t* str = new wchar_t[strLen + 1];
                for (int j = 0; j < strLen; ++j)
                {
                    str[j] = input->get(2 + iDims + offset + 1 + j);
                }
                str[strLen] = '\0';
                pString->set(i, str);
                delete[] str;

                offset += 1 + strLen;
            }

            out.push_back(pString);
            break;
        }

        default :
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Unknown type.\n"), funname.c_str(), 1);
            return types::Function::Error;
        }
    }

    return types::Function::OK;
}
