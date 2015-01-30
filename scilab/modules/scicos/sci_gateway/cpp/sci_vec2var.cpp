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

const std::string funname = "vec2var";

/**
 * Calculate the length increment depending on the type of the Scilab type
 *
 * @param T Scilab type
 * @param v the instance on the Scilab type
 * @return the number of double used to store the data
 */
template<typename T>
static size_t required_length(T* v)
{
    const size_t sizeof_double = sizeof(double);
    if (sizeof(typename T::type) >= sizeof_double)
    {
        return v->getSize() * sizeof(typename T::type) / sizeof_double;
    }
    else
    {
        // Increase the size to contain enough space, manage the size_t rounding issue
        return (v->getSize() * sizeof(typename T::type) + (sizeof_double - 1)) / sizeof_double;
    }
}

template<typename T>
static int fill_int(const double* const tab, const int iDims, T* &res)
{
    int  iElements = 1;
    int* pDims = new int[iDims];
    for (int i = 0; i < iDims; ++i)
    {
        pDims[i] = tab[i];
        iElements *= pDims[i];
    }
    res = new T(iDims, pDims);
    delete[] pDims;

    int numberOfDoubleNeeded = required_length(res);

    //Use a buffer to prevent copying only parts of integers
    double* buffer = new double[numberOfDoubleNeeded];
    memcpy(buffer, tab + iDims, numberOfDoubleNeeded * sizeof(double));
    memcpy(res->get(), buffer, iElements * sizeof(typename T::type));
    delete[] buffer;

    return 2 + iDims + numberOfDoubleNeeded;
}

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

    // Retrieving the first value pointer
    double* strData = const_cast<double*>(tab + iDims + iElements);

    res->set(0, (char*) & (*strData));
    strData += static_cast<size_t>(tab[iDims]);
    int offset = static_cast<size_t>(tab[iDims]);
    for (int i = 1; i < iElements; i++)
    {
        res->set(i, (char*) & (*strData));

        // Increment the value pointer by the number of elements
        const size_t numberOfElem = static_cast<size_t>(tab[iDims + i]) - static_cast<size_t>(tab[iDims + i - 1]);
        strData += numberOfElem;
        offset += static_cast<int>(numberOfElem);
    }

    return 2 + iDims + iElements + offset;
}

template<typename T>
static bool fill_list(const double* const tab, const int iDims, T* &res)
{
    res = new T();
    int offset = 0;
    for (int i = 0; i < iDims; ++i)
    {
        switch (static_cast<int>(*(tab + offset)))
        {
            case types::InternalType::ScilabDouble :
            {
                types::Double* pDouble;
                offset += fill_double(tab + 2 + offset, *(tab + 1 + offset), pDouble);
                res->append(pDouble);
                break;
            }
            case types::InternalType::ScilabInt8   :
            {
                types::Int8* pInt8;
                offset += fill_int(tab + 2 + offset, *(tab + 1 + offset), pInt8);
                res->append(pInt8);
                break;
            }
            case types::InternalType::ScilabUInt8  :
            {
                types::UInt8* pUInt8;
                offset += fill_int(tab + 2 + offset, *(tab + 1 + offset), pUInt8);
                res->append(pUInt8);
                break;
            }
            case types::InternalType::ScilabInt16  :
            {
                types::Int16* pInt16;
                offset += fill_int(tab + 2 + offset, *(tab + 1 + offset), pInt16);
                res->append(pInt16);
                break;
            }
            case types::InternalType::ScilabUInt16 :
            {
                types::UInt16* pUInt16;
                offset += fill_int(tab + 2 + offset, *(tab + 1 + offset), pUInt16);
                res->append(pUInt16);
                break;
            }
            case types::InternalType::ScilabInt32  :
            {
                types::Int32* pInt32;
                offset += fill_int(tab + 2 + offset, *(tab + 1 + offset), pInt32);
                res->append(pInt32);
                break;
            }
            case types::InternalType::ScilabUInt32 :
            {
                types::UInt32* pUInt32;
                offset += fill_int(tab + 2 + offset, *(tab + 1 + offset), pUInt32);
                res->append(pUInt32);
                break;
            }
            case types::InternalType::ScilabInt64  :
            {
                types::Int64* pInt64;
                offset += fill_int(tab + 2 + offset, *(tab + 1 + offset), pInt64);
                res->append(pInt64);
                break;
            }
            case types::InternalType::ScilabUInt64 :
            {
                types::UInt64* pUInt64;
                offset += fill_int(tab + 2 + offset, *(tab + 1 + offset), pUInt64);
                res->append(pUInt64);
                break;
            }
            case types::InternalType::ScilabBool   :
            {
                types::Bool* pBool;
                offset += fill_int(tab + 2 + offset, *(tab + 1 + offset), pBool);
                res->append(pBool);
                break;
            }
            case types::InternalType::ScilabString :
            {
                types::String* pString;
                offset += fill_string(tab + 2 + offset, *(tab + 1 + offset), pString);
                res->append(pString);
                break;
            }
            default :
            {
                Scierror(999, _("%s: Wrong value for element #%d of input argument #%d: Unknown type.\n"), funname.c_str(), 3 + offset, 1);
                res->DecreaseRef();
                res->killMe();
                return false;
            }
        }
    }
    return true;
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
            fill_int(input->get() + 2, iDims, pInt8);

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
            fill_int(input->get() + 2, iDims, pUInt8);

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
            fill_int(input->get() + 2, iDims, pInt16);

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
            fill_int(input->get() + 2, iDims, pUInt16);

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
            fill_int(input->get() + 2, iDims, pInt32);

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
            fill_int(input->get() + 2, iDims, pUInt32);

            out.push_back(pUInt32);
            break;
        }

        case types::InternalType::ScilabInt64  :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::Int64* pInt64;
            fill_int(input->get() + 2, iDims, pInt64);

            out.push_back(pInt64);
            break;
        }

        case types::InternalType::ScilabUInt64 :
        {
            if (input->getRows() < 4)
            {
                Scierror(999, _("%s:  Wrong size for input argument #%d: At least %dx%d elements expected.\n"), funname.c_str(), 1, 4, 1);
                return types::Function::Error;
            }

            types::UInt64* pUInt64;
            fill_int(input->get() + 2, iDims, pUInt64);

            out.push_back(pUInt64);
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
            fill_int(input->get() + 2, iDims, pBool);

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
            types::List* pList;
            if (!fill_list(input->get() + 2, iDims, pList))
            {
                return types::Function::Error;
            }

            out.push_back(pList);
            break;
        }

        case types::InternalType::ScilabTList  :
        {
            types::TList* pTList;
            if (!fill_list(input->get() + 2, iDims, pTList))
            {
                return types::Function::Error;
            }

            out.push_back(pTList);
            break;
        }
        case types::InternalType::ScilabMList  :
        {
            types::MList* pMList;
            if (!fill_list(input->get() + 2, iDims, pMList))
            {
                return types::Function::Error;
            }

            out.push_back(pMList);
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
