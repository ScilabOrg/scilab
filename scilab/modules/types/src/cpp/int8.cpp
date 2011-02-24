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
    Int8::~Int8()
    {
        if(isDeletable() == true)
        {
            deleteAll();
        }
    }

    Int8::Int8(int _iRows, int _iCols)
    {
        int piDims[2]   = {_iRows, _iCols};
        char *pcInt     = NULL;
        create(piDims, 2, &pcInt, NULL);
    }

    Int8::Int8(char _cVal)
    {
        int piDims[2]   = {1, 1};
        char *pcInt     = NULL;
        create(piDims, 2, &pcInt, NULL);
        pcInt[0] = _cVal;
    }

    Int8::Int8(int _iRows, int _iCols, char** _pcData)
    {
        int piDims[2] = {_iRows, _iCols};
        create(piDims, 2, _pcData, NULL);
    }

    Int8::Int8(int _iDims, int* _piDims)
    {
        char *pcInt     = NULL;
        create(_piDims, _iDims, &pcInt, NULL);
    }

    InternalType* Int8::clone()
    {
        Int8 *pbClone =  new Int8(getDims(), getDimsArray());
        pbClone->set(get());
        return pbClone;
    }

    void Int8::whoAmI()
    {
        std::cout << "types::Int8";
    }

    void Int8::subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen)
    {
        ostr << std::endl;
        /*Comment tenir compte de la longueur des lignes dans le formatage de variable ? */
        if(isScalar())
        {//scalar
            int iWidth  = 0;
            _piDims[0]  = 0;
            _piDims[1]  = 0;
            int iPos    = getIndex(_piDims);

            getSignedIntFormat(get(iPos), &iWidth);
            addSignedIntValue(&ostr, get(iPos), iWidth);
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

                getSignedIntFormat(get(iPos), &iWidth);
                addSignedIntValue(&ostr, get(iPos), iWidth);
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

                getSignedIntFormat(get(iPos), &iWidth);
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

                addSignedIntValue(&ostemp, get(iPos), iWidth);
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

                    getSignedIntFormat(get(iPos), &iWidth);
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

                            getSignedIntFormat(get(iPos), &iWidth);
                            addSignedIntValue(&ostemp, get(iPos), iWidth);
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

                    getSignedIntFormat(get(iPos), &iWidth);
                    addSignedIntValue(&ostemp, get(iPos), iWidth);
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

    bool Int8::operator==(const InternalType& it)
    {
        if(const_cast<InternalType &>(it).isInt8() == false)
        {
            return false;
        }

        Int8* pb = const_cast<InternalType &>(it).getAs<types::Int8>();

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

    bool Int8::operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    char Int8::getNullValue()
    {
        return 0;
    }

    Int8* Int8::createEmpty(int _iDims, int* _piDims, bool _bComplex)
    {
        return new Int8(_iDims, _piDims);
    }

    char Int8::copyValue(char _cData)
    {
        return _cData;
    }

    void Int8::deleteAll()
    {
        delete[] m_pRealData;
        m_pRealData = NULL;
        deleteImg();
    }

    void Int8::deleteImg()
    {
    }

    char* Int8::allocData(int _iSize)
    {
        return new char[_iSize];
    }
}

