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
#include "list.hxx"
#include "tlist.hxx"
#include "mlist.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

const std::string funname = "var2vec";

types::Function::ReturnValue sci_var2vec(types::typed_list &in, int _iRetCount, types::typed_list &out)
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

    // Size and content of the returned column vector
    int totalSize;
    types::Double* ret;

    switch (in[0]->getType())
    {
            // Reuse scicos model encoding for 'model.opar' and 'model.odstate' fields
        case types::InternalType::ScilabDouble :
        {
            types::Double* pDouble = in[0]->getAs<types::Double>();

            int  isComplex = (pDouble->isComplex()) ? 1 : 0;
            int  iDims = pDouble->getDims();
            int* pDims = pDouble->getDimsArray();
            int  iElements = 1;
            for (int i = 0; i < iDims; ++i)
            {
                iElements *= pDims[i];
            }
            totalSize = 2 + iDims + 1 + (1 + isComplex) * iElements;
            // Allocation for type + number of dimensions + each dimension + complex boolean + each element (doubled if complex)
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabDouble);
            ret->set(1, iDims);
            for (int i = 0; i < iDims; ++i)
            {
                ret->set(2 + i, pDims[i]);
            }
            ret->set(2 + iDims, isComplex);

            memcpy(ret->get() + 2 + iDims + 1, pDouble->getReal(), iElements * sizeof(double));
            if (isComplex == true)
            {
                memcpy(ret->get() + 2 + iDims + 1 + iElements, pDouble->getImg(), iElements * sizeof(double));
            }
            // An empty matrix input will return [12; 2; 0; 0; 0]

            break;
        }

        case types::InternalType::ScilabInt8   :
        {
            types::Int8* pInt8 = in[0]->getAs<types::Int8>();

            int  iDims = pInt8->getDims();
            int* pDims = pInt8->getDimsArray();
            int  iElements = 1;
            for (int i = 0; i < iDims; ++i)
            {
                iElements *= pDims[i];
            }
            // It takes 1 double (8 bytes) to save 8 char (1 byte)
            // So reserve 'iElements/8' and '2+iDims' integers for the type and matrix dimensions
            int numberOfIntNeeded = ((iElements - 1) / 8) + 1;
            totalSize = 2 + iDims + numberOfIntNeeded;
            // Allocation for type + number of dimensions + each dimension + each element
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabInt8);
            ret->set(1, iDims);
            for (int i = 0; i < iDims; ++i)
            {
                ret->set(2 + i, pDims[i]);
            }

            // Using contiguity of the memory, we save the input into 'ret'
            // Use a buffer to fill the entirety of 'ret'
            char* buffer = new char[numberOfIntNeeded * 8];
            memcpy(buffer, pInt8->get(), iElements * sizeof(char));
            memcpy(ret->get() + 2 + iDims, buffer, numberOfIntNeeded * sizeof(double));
            delete[] buffer;
            break;
        }

        case types::InternalType::ScilabUInt8  :
        {
            types::UInt8* pUInt8 = in[0]->getAs<types::UInt8>();

            int  iDims = pUInt8->getDims();
            int* pDims = pUInt8->getDimsArray();
            int  iElements = 1;
            for (int i = 0; i < iDims; ++i)
            {
                iElements *= pDims[i];
            }
            // It takes 1 double (8 bytes) to save 8 unsigned char (1 byte)
            // So reserve 'iElements/8' and '2+iDims' integers for the type and matrix dimensions
            int numberOfIntNeeded = ((iElements - 1) / 8) + 1;
            totalSize = 2 + iDims + numberOfIntNeeded;
            // Allocation for type + number of dimensions + each dimension + each element
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabUInt8);
            ret->set(1, iDims);
            for (int i = 0; i < iDims; ++i)
            {
                ret->set(2 + i, pDims[i]);
            }

            // Using contiguity of the memory, we save the input into 'ret'
            // Use a buffer to fill the entirety of 'ret'
            unsigned char* buffer = new unsigned char[numberOfIntNeeded * 8];
            memcpy(buffer, pUInt8->get(), iElements * sizeof(unsigned char));
            memcpy(ret->get() + 2 + iDims, buffer, numberOfIntNeeded * sizeof(double));
            delete[] buffer;
            break;
        }

        case types::InternalType::ScilabInt16  :
        {
            types::Int16* pInt16 = in[0]->getAs<types::Int16>();

            int  iDims = pInt16->getDims();
            int* pDims = pInt16->getDimsArray();
            int  iElements = 1;
            for (int i = 0; i < iDims; ++i)
            {
                iElements *= pDims[i];
            }
            // It takes 1 double (8 bytes) to save 4 short int (2 bytes)
            // So reserve 'iElements/4' and '2+iDims' integers for the type and matrix dimensions
            int numberOfIntNeeded = ((iElements - 1) / 4) + 1;
            totalSize = 2 + iDims + numberOfIntNeeded;
            // Allocation for type + number of dimensions + each dimension + each element
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabInt16);
            ret->set(1, iDims);
            for (int i = 0; i < iDims; ++i)
            {
                ret->set(2 + i, pDims[i]);
            }

            // Using contiguity of the memory, we save the input into 'ret'
            // Use a buffer to fill the entirety of 'ret'
            short int* buffer = new short int[numberOfIntNeeded * 4];
            memcpy(buffer, pInt16->get(), iElements * sizeof(short int));
            memcpy(ret->get() + 2 + iDims, buffer, numberOfIntNeeded * sizeof(double));
            delete[] buffer;
            break;
        }

        case types::InternalType::ScilabUInt16 :
        {
            types::UInt16* pUInt16 = in[0]->getAs<types::UInt16>();

            int  iDims = pUInt16->getDims();
            int* pDims = pUInt16->getDimsArray();
            int  iElements = 1;
            for (int i = 0; i < iDims; ++i)
            {
                iElements *= pDims[i];
            }
            // It takes 1 double (8 bytes) to save 4 unsigned short int (2 bytes)
            // So reserve 'iElements/4' and '2+iDims' integers for the type and matrix dimensions
            int numberOfIntNeeded = ((iElements - 1) / 4) + 1;
            totalSize = 2 + iDims + numberOfIntNeeded;
            // Allocation for type + number of dimensions + each dimension + each element
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabUInt16);
            ret->set(1, iDims);
            for (int i = 0; i < iDims; ++i)
            {
                ret->set(2 + i, pDims[i]);
            }

            // Using contiguity of the memory, we save the input into 'ret'
            // Use a buffer to fill the entirety of 'ret'
            unsigned short int* buffer = new unsigned short int[numberOfIntNeeded * 4];
            memcpy(buffer, pUInt16->get(), iElements * sizeof(unsigned short int));
            memcpy(ret->get() + 2 + iDims, buffer, numberOfIntNeeded * sizeof(double));
            delete[] buffer;
            break;
        }

        case types::InternalType::ScilabInt32  :
        {
            types::Int32* pInt32 = in[0]->getAs<types::Int32>();

            int  iDims = pInt32->getDims();
            int* pDims = pInt32->getDimsArray();
            int  iElements = 1;
            for (int i = 0; i < iDims; ++i)
            {
                iElements *= pDims[i];
            }
            // It takes 1 double (8 bytes) to save 2 int (4 bytes)
            // So reserve 'iElements/2' and '2+iDims' integers for the type and matrix dimensions
            int numberOfIntNeeded = ((iElements - 1) / 2) + 1;
            totalSize = 2 + iDims + numberOfIntNeeded;
            // Allocation for type + number of dimensions + each dimension + each element
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabInt32);
            ret->set(1, iDims);
            for (int i = 0; i < iDims; ++i)
            {
                ret->set(2 + i, pDims[i]);
            }

            // Using contiguity of the memory, we save the input into 'ret'
            // Use a buffer to fill the entirety of 'ret'
            int* buffer = new int[numberOfIntNeeded * 2];
            memcpy(buffer, pInt32->get(), iElements * sizeof(int));
            memcpy(ret->get() + 2 + iDims, buffer, numberOfIntNeeded * sizeof(double));
            delete[] buffer;
            break;
        }

        case types::InternalType::ScilabUInt32 :
        {
            types::UInt32* pUInt32 = in[0]->getAs<types::UInt32>();

            int  iDims = pUInt32->getDims();
            int* pDims = pUInt32->getDimsArray();
            int  iElements = 1;
            for (int i = 0; i < iDims; ++i)
            {
                iElements *= pDims[i];
            }
            // It takes 1 double (8 bytes) to save 2 unsigned int (4 bytes)
            // So reserve 'iElements/2' and '2+iDims' integers for the type and matrix dimensions
            int numberOfIntNeeded = ((iElements - 1) / 2) + 1;
            totalSize = 2 + iDims + numberOfIntNeeded;
            // Allocation for type + number of dimensions + each dimension + each element
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabUInt32);
            ret->set(1, iDims);
            for (int i = 0; i < iDims; ++i)
            {
                ret->set(2 + i, pDims[i]);
            }

            // Using contiguity of the memory, we save the input into 'ret'
            // Use a buffer to fill the entirety of 'ret'
            unsigned int* buffer = new unsigned int[numberOfIntNeeded * 2];
            memcpy(buffer, pUInt32->get(), iElements * sizeof(unsigned int));
            memcpy(ret->get() + 2 + iDims, buffer, numberOfIntNeeded * sizeof(double));
            delete[] buffer;
            break;
        }

        case types::InternalType::ScilabBool   :
        {
            types::Bool* pBool = in[0]->getAs<types::Bool>();

            int  iDims = pBool->getDims();
            int* pDims = pBool->getDimsArray();
            int  iElements = 1;
            for (int i = 0; i < iDims; ++i)
            {
                iElements *= pDims[i];
            }
            totalSize = 2 + iDims + iElements;
            // Allocation for type + number of dimensions + each dimension + each element
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabBool);
            ret->set(1, iDims);
            for (int i = 0; i < iDims; ++i)
            {
                ret->set(2 + i, pDims[i]);
            }
            for (int i = 0; i < iElements; ++i)
            {
                ret->set(2 + iDims + i, (pBool->get(i)) ? 1 : 0);
            }

            break;
        }

        case types::InternalType::ScilabString :
        {
            types::String* pString = in[0]->getAs<types::String>();

            int  iDims = pString->getDims();
            int* pDims = pString->getDimsArray();
            int  iElements = 1;
            for (int i = 0; i < iDims; ++i)
            {
                iElements *= pDims[i];
            }
            totalSize = 2 + iDims + iElements;

            // Reserve space for the string offsets and contents
            char** utf8 = new char*[iElements];
            size_t* pLengths = new size_t[iElements];
            int* offsets = new int[iElements];
            int offset_it = 0;
            for (int i = 0; i < iElements; ++i)
            {
                char* str = wide_string_to_UTF8(pString->get(i));
                utf8[i] = str;
                // Adding the '\0' byte to the length
                const size_t len = strlen(str) + 1;
                pLengths[i] = len;

                offset_it += (len * sizeof(char) + sizeof(double) - 1) / sizeof(double);
                offsets[i] = offset_it;
                totalSize += offset_it;
            }

            // Allocation for type + number of dimensions + each dimension + each element length + each element
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabString);
            ret->set(1, iDims);
            for (int i = 0; i < iDims; ++i)
            {
                ret->set(2 + i, pDims[i]);
            }

            size_t len = pLengths[0];
            ret->set(2 + iDims, offsets[0]);
            memcpy(ret->get() + 2 + iDims + iElements, *utf8, len * sizeof(char));
            for (int i = 1; i < iElements; ++i)
            {
                len = pLengths[i];
                ret->set(2 + iDims + i, offsets[i]);
                memcpy(ret->get() + 2 + iDims + iElements + offsets[i - 1], *(utf8 + i), len * sizeof(char));
            }

            // Free all the strings, after being copied
            for (int i = 0; i < iElements; ++i)
            {
                FREE(utf8[i]);
            }
            delete[] utf8;
            delete[] offsets;
            delete[] pLengths;
            break;
        }

        case types::InternalType::ScilabList   :
        {
            types::List* pList = in[0]->getAs<types::List>();

            int iElements = pList->getSize();
            totalSize = 2;

            std::vector<types::typed_list> listElements (iElements);
            for (int i = 0; i < iElements; ++i)
            {
                // Recursively call sci_var2vec on each element and extract the obtained results
                types::typed_list tempIn (1, pList->get(i));
                sci_var2vec(tempIn, 1, listElements[i]);

                types::Double* listElement = listElements[i][0]->getAs<types::Double>();
                totalSize += listElement->getRows();
            }
            // Allocation for type + list length + each list element
            ret = new types::Double(totalSize, 1);

            ret->set(0, types::InternalType::ScilabList);
            ret->set(1, iElements);
            int offset = 0;
            for (int i = 0; i < iElements; ++i)
            {
                types::Double* listElement = listElements[i][0]->getAs<types::Double>();
                memcpy(ret->get() + 2 + offset, listElement->get(), listElement->getRows() * sizeof(double));
                offset += listElement->getRows();
            }
            // An empty list input will return [22; 0]

            break;
        }

        case types::InternalType::ScilabTList  :
        case types::InternalType::ScilabMList  :
        {
            types::List* pList;
            if (in[0]->getType() == types::InternalType::ScilabTList)
            {
                pList = in[0]->getAs<types::TList>();
            }
            else
            {
                pList = in[0]->getAs<types::MList>();
            }

            int iElements = pList->getSize();
            totalSize = 2;

            std::vector<types::typed_list> listElements (iElements);
            for (int i = 0; i < iElements; ++i)
            {
                // Recursively call sci_var2vec on each element and extract the obtained results
                types::typed_list tempIn (1, pList->get(i));
                sci_var2vec(tempIn, 1, listElements[i]);

                types::Double* listElement = listElements[i][0]->getAs<types::Double>();
                totalSize += listElement->getRows();
            }
            // Allocation for type + list length + each list element
            ret = new types::Double(totalSize, 1);

            ret->set(0, in[0]->getType());
            ret->set(1, iElements);
            int offset = 0;
            for (int i = 0; i < iElements; ++i)
            {
                types::Double* listElement = listElements[i][0]->getAs<types::Double>();
                memcpy(ret->get() + 2 + offset, listElement->get(), listElement->getRows() * sizeof(double));
                offset += listElement->getRows();
            }

            break;
        }

        default :
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Unknown type.\n"), funname.c_str(), 1);
            return types::Function::Error;
        }
    }

    out.push_back(ret);
    return types::Function::OK;
}
