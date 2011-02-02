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

// This code is separated in bool.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

#ifndef __ARRAYOF_HXX__
    #error This file must only be include by arrayof.hxx
#endif

namespace types
{
    class Bool : public GenericType
    {
    public :
                                Bool(int _bReal);
                                Bool(int _iRows, int _iCols);
                                Bool(int _iRows, int _iCols, int **_piData);
                                ~Bool();

		InternalType*           clone();

		/*data management*/
		int*                    bool_get() const;
		int                     bool_get(int _iRows, int _iCols) const;

		bool                    bool_set(int *_piData);
		bool                    bool_set(int _iRows, int _iCols, int _iData);

        GenericType*            getColumnValues(int _iPos);


		/*zero or one set filler*/
		bool                    setFalse();
		bool                    setTrue();

		/*Config management*/
        void                    whoAmI();
		bool                    isComplex();

        Bool*                   getAsBool(void);
        std::wstring            toString(int _iPrecision, int _iLineLen);

		bool                    resize(int _iNewRows, int _iNewCols);
		InternalType*           insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, GenericType* _poSource, bool _bAsVector);
		static Bool*            insertNew(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, Bool* _poSource, bool _bAsVector);
		Bool*                   extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, int* _piDimSize, bool _bAsVector);

		bool                    operator==(const InternalType& it);
		bool                    operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring     getTypeStr() {return L"boolean";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring     getShortTypeStr() {return L"b";}

    protected :
		RealType				getType(void);

    private :
		/*clean values array*/
		void                    deleteAll();
		void                    CreateBool(int _iRows, int _iCols, int **_ibData);


    private :
        int*                    m_piData;
    };
}

