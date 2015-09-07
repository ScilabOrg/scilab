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

#include <vector>
#include <string>
#include <cwchar>
#include <cstring>

#include "var2vec.hxx"

#include "types.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "string.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
#include "struct.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

static const std::string var2vecName = "var2vec";

static void computeDims(types::GenericType* input, int &iDims, int* &pDims, int &iElements, int &totalSize)
{
    iDims = input->getDims();
    pDims = input->getDimsArray();
    iElements = 1;
    for (int i = 0; i < iDims; ++i)
    {
        iElements *= pDims[i];
    }

    // Type + number of dimensions + dimensions
    totalSize = 2 + iDims;
}

/**
 * Calculate the length increment depending on the ::value_type of 'V' and the type of the Scilab type 'T'
 *
 * @param V variable that must have a 'value_type' field
 * @param T Scilab type
 * @param v the instance of the Scilab type
 * @return the number of V elements used to store the data
 */
template<typename V, typename T>
size_t required_length(const V /*it*/, T* v)
{
    const size_t sizeof_ret_value = sizeof(typename V::value_type);
    if (sizeof(typename T::type) >= sizeof_ret_value)
    {
        return v->getSize() * sizeof(typename T::type) / sizeof_ret_value;
    }
    else
    {
        // Increase the size to contain enough space, manage the size_t rounding issue
        return (v->getSize() * sizeof(typename T::type) + (sizeof_ret_value - 1)) / sizeof_ret_value;
    }
}

template <typename T>
void encode(T* input, std::vector<double> &ret)
{
    int iDims, iElements, totalSize;
    int* pDims;
    computeDims(input, iDims, pDims, iElements, totalSize);

    const int nDoubleNeeded = static_cast<int>(required_length(ret, input));
    totalSize += nDoubleNeeded;

    // Allocation for type + number of dimensions + each dimension + each element
    ret.reserve(ret.size() + totalSize);

    ret.push_back(((types::InternalType*) input)->getType());
    ret.push_back(iDims);
    for (int i = 0; i < iDims; ++i)
    {
        ret.push_back(pDims[i]);
    }

    // Using contiguity of the memory, we save the input into 'ret'
    // Use a buffer to fill the entirety of 'ret'
    size_t size = ret.size();
    ret.resize(size + nDoubleNeeded);
    double* data = ret.data() + size;
    memcpy(data, input->get(), iElements * sizeof(typename T::type));
}

static void encode(types::Double* input, std::vector<double> &ret)
{
    int iDims, iElements, totalSize;
    int* pDims;
    computeDims(input, iDims, pDims, iElements, totalSize);

    const int isComplex = (input->isComplex()) ? 1 : 0;
    totalSize++; // Add the complex boolean
    totalSize += (isComplex + 1) * iElements; // Size of the required data buffer

    // Allocation for type + number of dimensions + each dimension + complex boolean + each element (doubled if complex)
    ret.reserve(ret.size() + totalSize);

    ret.push_back(types::InternalType::ScilabDouble);
    ret.push_back(iDims);
    for (int i = 0; i < iDims; ++i)
    {
        ret.push_back(pDims[i]);
    }
    ret.push_back(isComplex);

    size_t size = ret.size();
    ret.resize(size + iElements * (isComplex + 1));
    double* data = ret.data() + size;
    memcpy(data, input->getReal(), iElements * sizeof(double));

    if (isComplex == 1)
    {
        memcpy(data + iElements, input->getImg(), iElements * sizeof(double));
    }

    // An empty matrix input will return [12; 2; 0; 0; 0]
}

static void encode(types::String* input, std::vector<double> &ret)
{
    int iDims, iElements, totalSize;
    int* pDims;
    computeDims(input, iDims, pDims, iElements, totalSize);

    totalSize += iElements;

    // Reserve space for the string offsets and contents
    char** utf8 = new char*[iElements];
    size_t* pLengths = new size_t[iElements];
    int* offsets = new int[iElements];
    int offset_cur = 0, offset_acc = 0;
    for (int i = 0; i < iElements; ++i)
    {
        char* str = wide_string_to_UTF8(input->get(i));
        utf8[i] = str;
        // Adding the '\0' byte to the length
        const size_t len = strlen(str) + 1;
        pLengths[i] = len;

        offset_cur = static_cast<int>((len * sizeof(char) + sizeof(double) - 1) / sizeof(double));
        totalSize += offset_cur;
        offset_acc += offset_cur;
        offsets[i] = offset_acc;
    }

    // Allocation for type + number of dimensions + each dimension + each element length + each element
    ret.reserve(ret.size() + totalSize);

    ret.push_back(types::InternalType::ScilabString);
    ret.push_back(iDims);
    for (int i = 0; i < iDims; ++i)
    {
        ret.push_back(pDims[i]);
    }
    for (int i = 0; i < iElements; ++i)
    {
        ret.push_back(offsets[i]);
    }

    size_t size = ret.size();
    ret.resize(size + offsets[iElements - 1]);
    double* data = ret.data() + size;

    size_t len = pLengths[0];
    memcpy(data, utf8[0], len * sizeof(char));
    data += offsets[0];
    for (int i = 1; i < iElements; ++i)
    {
        size_t len = pLengths[i];
        memcpy(data, utf8[i], len * sizeof(char));
        data += offsets[i] - offsets[i - 1];
    }

    // Free all the strings, after being copied
    for (int i = 0; i < iElements; ++i)
    {
        FREE(utf8[i]);
    }
    delete[] utf8;
    delete[] offsets;
    delete[] pLengths;
}

static void encode(types::List* input, std::vector<double> &ret)
{
    const int iElements = input->getSize();

    ret.push_back(input->getType());
    ret.push_back(iElements);
    for (int i = 0; i < iElements; ++i)
    {
        // Recursively call var2vec on each element and extract the obtained results
        var2vec(input->get(i), ret);
    }

    // An empty list input will return [22; 0], a tlist [23; 0] and an mlist [24; 0]
}

static void encode(types::Struct* input, std::vector<double> &ret)
{
    const bool isEmpty = input->getSize() == 0;

    types::String* fields = nullptr;
    int iElements = 0;
    if (!isEmpty)
    {
        fields = input->getFieldNames();
        iElements = fields->getSize();
    }

    ret.push_back(input->getType());
    ret.push_back(iElements);
    if (!isEmpty)
    {
        // Call var2vec on the struct's fields to extract a header
        var2vec(fields, ret);

        // Now extract the fields' content, assuming this is not a multidimensional struct
        types::SingleStruct* content = input->get(0);
        for (int i = 0; i < iElements; ++i)
        {
            var2vec(content->get(fields->get(i)), ret);
        }


        fields->killMe();
    }

    // An empty struct input will return [26; 0]
}

bool var2vec(types::InternalType* in, std::vector<double> &out)
{
    switch (in->getType())
    {
        // Reuse scicos model encoding for 'model.opar' and 'model.odstate' fields
        case types::InternalType::ScilabDouble :
            encode(in->getAs<types::Double>(), out);
            break;

        case types::InternalType::ScilabInt8   :
            encode(in->getAs<types::Int8>(), out);
            break;
        case types::InternalType::ScilabUInt8  :
            encode(in->getAs<types::UInt8>(), out);
            break;
        case types::InternalType::ScilabInt16  :
            encode(in->getAs<types::Int16>(), out);
            break;
        case types::InternalType::ScilabUInt16 :
            encode(in->getAs<types::UInt16>(), out);
            break;
        case types::InternalType::ScilabInt32  :
            encode(in->getAs<types::Int32>(), out);
            break;
        case types::InternalType::ScilabUInt32 :
            encode(in->getAs<types::UInt32>(), out);
            break;
        case types::InternalType::ScilabInt64  :
            encode(in->getAs<types::Int64>(), out);
            break;
        case types::InternalType::ScilabUInt64 :
            encode(in->getAs<types::UInt64>(), out);
            break;
        case types::InternalType::ScilabBool   :
            encode(in->getAs<types::Bool>(), out);
            break;

        case types::InternalType::ScilabString :
            encode(in->getAs<types::String>(), out);
            break;

        case types::InternalType::ScilabList   :
            encode(in->getAs<types::List>(), out);
            break;
        case types::InternalType::ScilabTList  :
            encode(in->getAs<types::List>(), out);
            break;
        case types::InternalType::ScilabMList  :
            encode(in->getAs<types::List>(), out);
            break;
        case types::InternalType::ScilabStruct :
            encode(in->getAs<types::Struct>(), out);
            break;

        default :
            Scierror(999, _("%s: Wrong type for input argument #%d: %s, %s, %s, %s, %s or %s type.\n"), var2vecName.c_str(), 1, "Double", "Integer", "Boolean", "String", "List", "Struct");
            return false;
    }

    return true;
}
