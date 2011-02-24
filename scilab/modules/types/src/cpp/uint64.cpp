/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <sstream>
#include "arrayof.hxx"
#include "tostring_common.hxx"

extern "C"
{
#include "core_math.h"
}

namespace types
{
    UInt64::~UInt64()
    {
        if(isDeletable() == true)
        {
            deleteAll();
        }
    }

    UInt64::UInt64(int _iRows, int _iCols)
    {
        int piDims[2]           = {_iRows, _iCols};
        unsigned long long *pullVal  = NULL;
        create(piDims, 2, &pullVal, NULL);
    }

    UInt64::UInt64(unsigned long long _ullVal)
    {
        int piDims[2]           = {1, 1};
        unsigned long long *pullVal  = NULL;
        create(piDims, 2, &pullVal, NULL);
        pullVal[0] = _ullVal;
    }

    UInt64::UInt64(int _iRows, int _iCols, unsigned long long** _pullVal)
    {
        int piDims[2]   = {_iRows, _iCols};
        create(piDims, 2, _pullVal, NULL);
    }

    UInt64::UInt64(int _iDims, int* _piDims)
    {
        unsigned long long *pullVal    = NULL;
        create(_piDims, _iDims, &pullVal, NULL);
    }

    InternalType* UInt64::clone()
    {
        UInt64 *pbClone =  new UInt64(getDims(), getDimsArray());
        pbClone->set(get());
        return pbClone;
    }

    void UInt64::whoAmI()
    {
        std::cout << "types::UInt64";
    }

    void UInt64::subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen)
    {
        ostr << std::endl;
        /*Comment tenir compte de la longueur des lignes dans le formatage de variable ? */
        if(isScalar())
        {//scalar
            int iWidth  = 0;
            _piDims[0]  = 0;
            _piDims[1]  = 0;
            int iPos    = getIndex(_piDims);

            getUnsignedIntFormat(get(iPos), &iWidth);
            addUnsignedIntValue(&ostr, get(iPos), iWidth);
            ostr << std::endl;
        }
        else if(getCols() == 1)
        {//column vector

            for(int i = 0 ; i < getRows() ; i++)
            {
                int iWidth  = 0;
                _piDims[1]  = 0;
                _piDims[0]  = i;
                int iPos    = getIndex(_piDims);

                getUnsignedIntFormat(get(iPos), &iWidth);
                addUnsignedIntValue(&ostr, get(iPos), iWidth);
                ostr << std::endl;
            }
        }
        else if(getRows() == 1)
        {//row vector
            std::wostringstream ostemp;
            int iLastVal = 0;

            for(int i = 0 ; i < getCols() ; i++)
            {
                int iWidth  = 0;
                int iLen    = 0;
                _piDims[0]  = 0;
                _piDims[1]  = i;
                int iPos    = getIndex(_piDims);

                getUnsignedIntFormat(get(iPos), &iWidth);
                iLen = iWidth + static_cast<int>(ostemp.str().size());
                if(iLen > _iLineLen)
                {//Max length, new line
                    ostr << std::endl << L"       column " << iLastVal + 1 << L" to " << i << std::endl << std::endl;
                    ostr << ostemp.str() << std::endl;
                    ostemp.str(L"");
                    iLastVal = i;
                }

                if(ostemp.str().size() != 0)
                {
                    ostemp << SPACE_BETWEEN_TWO_VALUES;
                }

                addUnsignedIntValue(&ostemp, get(iPos), iWidth);
            }

            if(iLastVal != 0)
            {
                ostr << std::endl << L"       column " << iLastVal + 1 << L" to " << getCols() << std::endl << std::endl;
            }
            ostemp << std::endl;
            ostr << ostemp.str();
        }
        else // matrix
        {
            std::wostringstream ostemp;
            int iLen = 0;
            int iLastCol = 0;

            //Array with the max printed size of each col
            int *piSize = new int[getCols()];
            memset(piSize, 0x00, getCols() * sizeof(int));

            //compute the row size for padding for each printed bloc.
            for(int iCols1 = 0 ; iCols1 < getCols() ; iCols1++)
            {
                for(int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
                {
                    int iWidth  = 0;
                    _piDims[0]  = iRows1;
                    _piDims[1]  = iCols1;
                    int iPos    = getIndex(_piDims);

                    getUnsignedIntFormat(get(iPos), &iWidth);
                    piSize[iCols1] = Max(piSize[iCols1], iWidth);
                }

                if(iLen + piSize[iCols1] > _iLineLen)
                {//find the limit, print this part
                    for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
                    {
                        for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                        {
                            int iWidth  = 0;
                            _piDims[0]  = iRows2;
                            _piDims[1]  = iCols2;
                            int iPos    = getIndex(_piDims);

                            getUnsignedIntFormat(get(iPos), &iWidth);
                            addUnsignedIntValue(&ostemp, get(iPos), iWidth);
                            ostemp << SPACE_BETWEEN_TWO_VALUES;
                        }
                        ostemp << std::endl;
                    }
                    iLen = 0;
                    ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << std::endl << std::endl;;
                    ostr << ostemp.str();
                    ostemp.str(L"");
                    iLastCol = iCols1;

                }
                iLen += piSize[iCols1] + SIGN_LENGTH + SIZE_BETWEEN_TWO_VALUES;
            }

            for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
            {
                for(int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
                {
                    int iWidth  = 0;
                    _piDims[0]  = iRows2;
                    _piDims[1]  = iCols2;
                    int iPos    = getIndex(_piDims);

                    getUnsignedIntFormat(get(iPos), &iWidth);
                    addUnsignedIntValue(&ostemp, get(iPos), iWidth);
                    ostemp << SPACE_BETWEEN_TWO_VALUES;
                }
                ostemp << std::endl;
            }
            if(iLastCol != 0)
            {
                ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << getCols() << std::endl << std::endl;
            }
            ostr << ostemp.str();
        }
    }

    bool UInt64::operator==(const InternalType& it)
    {
        if(const_cast<InternalType &>(it).isUInt64() == false)
        {
            return false;
        }

        UInt64* pb = const_cast<InternalType &>(it).getAs<types::UInt64>();

        if(pb->getDims() != getDims())
        {
            return false;
        }

        for(int i = 0 ; i < getDims() ; i++)
        {
            if(pb->getDimsArray()[i] != getDimsArray()[i])
            {
                return false;
            }
        }

        if(memcmp(get(), pb->get(), getSize() * sizeof(int)) != 0)
        {
            return false;
        }
        return true;
    }

    bool UInt64::operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    unsigned long long UInt64::getNullValue()
    {
        return 0;
    }

    UInt64* UInt64::createEmpty(int _iDims, int* _piDims, bool _bComplex)
    {
        return new UInt64(_iDims, _piDims);
    }

    unsigned long long UInt64::copyValue(unsigned long long _ullVal)
    {
        return _ullVal;
    }

    void UInt64::deleteAll()
    {
        delete[] m_pRealData;
        m_pRealData = NULL;
        deleteImg();
    }

    void UInt64::deleteImg()
    {
    }

    unsigned long long* UInt64::allocData(int _iSize)
    {
        return new unsigned long long[_iSize];
    }
}

