/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This code is separated in string.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

#ifndef __ARRAYOF_HXX__
    #error This file must only be include by arrayof.hxx
#endif

#include <string>
#include <list>

namespace types
{
    class String : public GenericType
    {
    public :
                                String(int _iRows, int _iCols);
                                String(int _iRows, int _iCols, wchar_t** _pstData);
                                String(const wchar_t *_pstData);
                            	String(const char *_pstData);
        virtual                 ~String();

        GenericType*            get_col_value(int _iPos);

        wchar_t**               string_get() const;
        wchar_t*                string_get(int _iRows, int _iCols) const;
        wchar_t*                string_get(int _iPos) const;

        bool                    string_set(wchar_t **_pstData);
        bool                    string_set(int _iRows, int _iCols, const wchar_t *_pstData);
        bool                    string_set(int _iPos, const wchar_t *_pstData);

        void                    whoAmI();

        String*                 getAsString(void);
        std::wstring            toString(int _iPrecision, int _iLineLen);

        bool                    resize(int _iNewRows, int _iNewCols);
        InternalType*           insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, GenericType* _poSource, bool _bAsVector);
        static String*          insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, String* _poSource, bool _bAsVector);
        String*                 extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, int* _piDimSize, bool _bAsVector);
        bool                    append(int _iRows, int _iCols, String *_poSource);

        bool                    operator==(const InternalType& it);
        bool                    operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring    getTypeStr() {return L"string";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring    getShortTypeStr() {return L"c";}
        InternalType*           clone();

    protected :
        RealType                getType();//			{ return RealString; }

    private :
        wchar_t**               m_pstData;

        void                    all_delete();
        void                    string_delete(int _iRows, int _iCols);
        void                    string_delete(int _iPos);

        void                    CreateString(int _iRows, int _iCols);
    };
}

