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

// This code is separated in matrixpoly.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

#ifndef __ARRAYOF_HXX__
    #error This file must only be include by arrayof.hxx
#endif
#include "poly.hxx"

#ifdef isComplex
#undef isComplex
#endif

namespace types
{
    class MatrixPoly : public GenericType
    {
    public :
                                MatrixPoly();
                                MatrixPoly(std::wstring _szVarName, int _iRows, int _iCols, int *_piRank);
                                MatrixPoly(std::wstring _szVarName, int _iRows, int _iCols, const int *_piRank);
        virtual                 ~MatrixPoly();

        // FIXME : Should not return NULL
        InternalType*           clone() { std::cout << "MatrixPoly::clone" << std::endl; return NULL; }

        GenericType*            getColumnValues(int _iPos);

        Poly*                   getPoly(int _iRows, int _iCols);
        Poly*                   getPoly(int _iIdx);
        bool                    poly_set(int _iRows, int _iCols, Double *_pdblCoef);
        bool                    poly_set(int _iIdx, Double *_pdblCoef);

        MatrixPoly*             getAsPoly();
        RealType                getType();
        bool                    isPoly() { return true; }
        void                    whoAmI(void);
        bool                    isComplex(void);
        void                    setComplex(bool _bComplex);
        std::wstring            getVariableName();
        bool                    getRank(int *_piRank);
        int                     getMaxRank(void);
        Double*                 evaluate(Double* _pdblValue);
        void                    updateRank(void);
        Double*                 getCoef(void);
        void                    setCoef(Double *_pCoef);
        Double*                 extractCoef(int _iRank);
        bool                    insertCoef(int _iRank, Double* _pCoef);

        InternalType*           insert(int _iRows, int _iCols, MatrixPoly* _poSource);

        std::wstring            toString(int _iPrecison, int _iLineLen);
        std::wstring            getRowString(int _iPrecison, int _iLineLen, bool _bComplex);
        std::wstring            getColString(int _iPrecison, int _iLineLen, bool _bComplex);
        std::wstring            getMatrixString(int _iPrecison, int _iLineLen, bool _bComplex);


        MatrixPoly&             operator= (MatrixPoly& poPoly);

        bool                    operator==(const InternalType& it);
        bool                    operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring    getTypeStr() {return L"matrixpoly";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring    getShortTypeStr() {return L"p";}

    protected :
        Poly*                   m_poPolyMatrix;
        bool                    m_bComplex;
        std::wstring            m_szVarName;
    };
}

