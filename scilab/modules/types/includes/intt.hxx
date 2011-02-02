/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file intt.hxx
 ** Define Scilab integer datatypes {u}int{8,16,32,64}
 */

#ifndef __INTT_HH__
#define __INTT_HH__

#include "core_math.h"
#include "types.hxx"
#include "tostring_common.hxx"
namespace types
{
    /**
     ** Class IntT<typename T>
     ** \brief This is a inner class that derivates from Int to define all integers
     ** \param typename T stands for the data representation
     **
     ** Scilab : uint8 -> 8 bytes -> C : unsigned char
     ** Scilab : int8 -> 8 bytes -> C : char
     ** ...
     ** Scilab : uint64 -> 64 bytes -> C : unsigned long long
     ** Scilab : int64 -> 64 bytes -> C : long long
     */

    template <typename T>
    class IntT : public Int
    {
        virtual void subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen)
        {
            if(getRows() == 1 && getCols() == 1)
            {//scalar
                int iWidth = 0;
                GetIntFormat(m_pData[0], &iWidth);
                AddIntValue(&ostr, m_pData[0], iWidth);
                ostr << std::endl;
            }
            else if(getCols() == 1)
            {//column vector
                for(int i = 0 ; i < getRows() ; i++)
                {
                    int iWidth = 0;
                    GetIntFormat(m_pData[i], &iWidth);
                    AddIntValue(&ostr, m_pData[i], iWidth);
                    ostr << std::endl;
                }
            }
            else if(getRows() == 1)
            {//row vector
                std::wostringstream ostemp;
                int iLastVal = 0;

                for(int i = 0 ; i < getCols() ; i++)
                {
                    int iWidth = 0;
                    GetIntFormat(m_pData[i], &iWidth);
                    int iLen = iWidth + static_cast<int>(ostemp.str().size());
                    if(iLen > _iLineLen)
                    {//Max length, new line
                        ostr << std::endl << L"       column " << iLastVal + 1 << L" to " << i << std::endl << std::endl;
                        ostr << ostemp.str() << std::endl;
                        ostemp.str(L"");
                        iLastVal = i;
                    }

                    AddIntValue(&ostemp, m_pData[i], iWidth);
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
                        int iWidth			= 0;
                        int iCurrentLen = 0;

                        GetIntFormat(m_pData[iCols1 * getRows() + iRows1], &iWidth);
                        iCurrentLen	= iWidth ;

                        if(iCurrentLen > piSize[iCols1])
                        {
                            piSize[iCols1] = iCurrentLen;
                        }
                    }

                    if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
                    {//find the limit, print this part
                        for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
                        {
                            for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                            {
                                int iWidth	= 0;
                                GetIntFormat(m_pData[iCols2 * getRows() + iRows2], &iWidth);
                                AddIntValue(&ostemp, m_pData[iCols2 * getRows() + iRows2], piSize[iCols2]);
                            }
                            ostemp << std::endl;
                        }
                        iLen = 0;
                        ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << std::endl << std::endl;;
                        ostr << ostemp.str();
                        ostemp.str(L"");
                        iLastCol = iCols1;

                    }
                    iLen += piSize[iCols1] + SIGN_LENGTH;
                }

                for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
                {
                    for(int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
                    {
                        int iWidth			= 0;
                        GetIntFormat(m_pData[iCols2 * getRows() + iRows2], &iWidth);
                        AddIntValue(&ostemp, m_pData[iCols2 * getRows() + iRows2], piSize[iCols2]);
                    }
                    ostemp << std::endl;
                }
                if(iLastCol != 0)
                {
                    ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << getCols() << std::endl << std::endl;
                }
                ostr << ostemp.str();
            }
            return ostr.str();
        }
        virtual IntType getIntType() = 0;

    private :
        virtual void AddIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign = false, bool bPrintOne = true) = 0;
        virtual void GetIntFormat(T _TVal, int *_piWidth) = 0;

    };

    /*
    ** Class SignedIntT<typename T>
    ** \brief This is a inner class that derivates from IntT to define all __signed__ integers
    ** \param typename T stands for the data representation and forwards IntT templated typename
    **
    ** Scilab : int8 -> 8 bytes -> C : char
    ** ...
    ** Scilab : int64 -> 64 bytes -> C : long long
    */
    template <typename T>
    class SignedIntT<T> : public Int<T>
    {
    protected :
        SignedIntT(int _iRows, int _iCols) : IntT<T>(_iRows, _iCols) {}

        void AddIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign, bool bPrintOne)
        {
            AddSignedIntValue(_postr, _TVal, _iWidth, bPrintPlusSign, bPrintOne);
        }

        void GetIntFormat(T _TVal, int *_piWidth)
        {
            GetSignedIntFormat(_TVal, _piWidth);
        }
    };

    /*
    ** Class SignedIntT<typename T>
    ** \brief This is a inner class that derivates from IntT to define all __unsisigned__ integers
    ** \param typename T stands for the data representation and forwards IntT templated typename
    **
    ** Scilab : uint8 -> 8 bytes -> C : unsigned char
    ** ...
    ** Scilab : uint64 -> 64 bytes -> C : unsigned long long
    */
    template <typename T>
    class UnsignedIntT<T> : public Int<T>
    {
    protected :
        UnsignedIntT(int _iRows, int _iCols) : IntT<T>(_iRows, _iCols) {}
        void AddIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign, bool bPrintOne)
        {
            AddUnsignedIntValue(_postr, _TVal, _iWidth, bPrintPlusSign, bPrintOne);
        }
        void GetIntFormat(T _TVal, int *_piWidth)
        {
            GetUnsignedIntFormat(_TVal, _piWidth);
        }
    };

    /*
    ** Class Int8 <-> Scilab int8(X)
    */
    class Int8 : public SignedIntT<char>
    {
    public :
        Int8(int _iRows, int _iCols) : SignedIntT<char>(_iRows, _iCols) {}
        IntType getIntType() { return IntType8; }
        std::wstring getTypeStr(){return L"int8";}
    };

    /*
    ** Class UInt8 <-> Scilab uint8(X)
    */
    class UInt8 : public UnsignedIntT<unsigned char>
    {
    public :
        UInt8(int _iRows, int _iCols) : UnsignedIntT<unsigned char>(_iRows, _iCols) {}
        IntType getIntType() { return TypeUnsigned8; }
        std::wstring getTypeStr(){return L"uint8";}
    };

//16 bits
    /*
    ** Class Int16 <-> Scilab int16(X)
    */
    class Int16 : public SignedIntT<short>
    {
    public :
        Int16(int _iRows, int _iCols) : SignedIntT<short>(_iRows, _iCols) {}
        IntType getIntType() { return Type16; }
        std::wstring getTypeStr(){return L"int16";}
    };

    /*
    ** Class UInt16 <-> Scilab uint16(X)
    */
    class UInt16 : public UnsignedIntT<unsigned short>
    {
    public :
        UInt16(int _iRows, int _iCols) : UnsignedIntT<unsigned short>(_iRows, _iCols) {}
        IntType getIntType() { return TypeUnsigned16; }
        std::wstring getTypeStr(){return L"uint16";}
    };

//32 bits
    /*
    ** Class Int32 <-> Scilab int32(X)
    */
    class Int32 : public SignedIntT<int>
    {
    public :
        Int32(int _iRows, int _iCols) : SignedIntT<int>(_iRows, _iCols) {}
        IntType getIntType() { return Type32; }
        std::wstring getTypeStr(){return L"int32";}
    };

    /*
    ** Class UInt32 <-> Scilab uint32(X)
    */
    class UInt32 : public UnsignedIntT<unsigned int>
    {
    public :
        UInt32(int _iRows, int _iCols) : UnsignedIntT<unsigned int>(_iRows, _iCols) {}
        IntType getIntType() { return TypeUnsigned32; }
        std::wstring getTypeStr(){return L"uint32";}
    };

//64 bits
    /*
    ** Class Int64 <-> Scilab int64(X) -*- NEW Scilab 6.0 -*-
    */
    class Int64 : public SignedIntT<long long>
    {
    public :
        Int64(int _iRows, int _iCols) : SignedIntT<long long>(_iRows, _iCols) {}
        IntType getIntType() { return Type64; }
        std::wstring getTypeStr(){return L"int64";}
    };

    /*
    ** Class UInt64 <-> Scilab uint64(X) -*- NEW Scilab 6.0 -*-
    */
    class UInt64 : public UnsignedIntT<unsigned long long>
    {
    public :
        UInt64(int _iRows, int _iCols) : UnsignedIntT<unsigned long long>(_iRows, _iCols) {}
        IntType getIntType() { return TypeUnsigned64; }
        std::wstring getTypeStr(){return L"uint64";}
    };
}
#endif /* __INTT_HH__ */
