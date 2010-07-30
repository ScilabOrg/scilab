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

#ifndef __MATRIXPOLY_HH__
#define __MATRIXPOLY_HH__

#include "poly.hxx"
#include "double.hxx"

#ifdef isComplex
#undef isComplex
#endif

namespace types
{
    class MatrixPoly : public GenericType
    {
    public :
                                MatrixPoly();
                                MatrixPoly(string _szVarName, int _iRows, int _iCols, int *_piRank);
        virtual                 ~MatrixPoly();

        // FIXME : Should not return NULL
        MatrixPoly*             clone() const{ return NULL; }

        GenericType*            get_col_value(int _iPos)const;

        Poly*                   poly_get(int _iRows, int _iCols);
        Poly*                   poly_get(int _iIdx);
        Poly const*                   poly_get(int _iRows, int _iCols) const;
        Poly const*                   poly_get(int _iIdx) const;
        bool                    poly_set(int _iRows, int _iCols, Double const*_pdblCoef);
        bool                    poly_set(int _iIdx, Double const*_pdblCoef);

        MatrixPoly*             getAsPoly();
        MatrixPoly const*             getAsPoly() const;
        RealType                getType() const;
        void                    whoAmI(void) const;
        bool                    isComplex(void) const;
        void                    complex_set(bool _bComplex);
        string                  var_get() const;
        bool                    rank_get(int *_piRank)const;
        int                     rank_max_get(void) const;
        Double*                 evaluate(Double* _pdblValue);
        void                    update_rank(void);
        Double*                 coef_get(void) const;
        void                    coef_set(Double const *_pCoef);
        Double*                 extract_coef(int _iRank) const;
        bool                    insert_coef(int _iRank, Double* _pCoef);

        bool                    insert(int _iRows, int _iCols, MatrixPoly* _poSource);

        string                  toString(int _iPrecison, int _iLineLen) const;
        string                  GetRowString(int _iPrecison, int _iLineLen, bool _bComplex) const;
        string                  GetColString(int _iPrecison, int _iLineLen, bool _bComplex) const;
        string                  GetMatrixString(int _iPrecison, int _iLineLen, bool _bComplex) const;


        MatrixPoly&             operator= (MatrixPoly const& poPoly);

        bool                    operator==(const InternalType& it) const;
        bool                    operator!=(const InternalType& it) const;

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string     getTypeStr() const {return string("matrixpoly");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string     getShortTypeStr() const {return string("p");}

    private :
        Poly*                   m_poPolyMatrix;
        bool                    m_bComplex;
        string                  m_szVarName;
    };
}
#endif /* !__MATRIXPOLY_HH__ */
