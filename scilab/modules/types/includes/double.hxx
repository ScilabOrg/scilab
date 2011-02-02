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

#ifndef __DOUBLE_HXX__
#define __DOUBLE_HXX__

#include "arrayof.hxx"

namespace types
{
    class Double : public ArrayOf<double>
    {
    public :
        virtual						~Double();

                                    Double(double _dblReal);
                                    Double(double _dblReal, double _dblImg);
                                    Double(int _iRows, int _iCols, bool _bComplex = false);
                                    Double(int _iRows, int _iCols, double **_pdblReal);
                                    Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);
                        	        Double(int _iDims, int* _piDims, bool _bComplex = false);

        static Double*              Empty();

        /*data management*/
        double*                     get_real() const;
        double                      get_real(int _iRows, int _iCols);
        bool                        set_real(double *_pdblReal);
        bool                        set_real(const double *_pdblReal);
        bool                        set_real(int* _piReal); //to translate int to double matrix
        bool                        set_real(int _iPos, double _dblReal);
        bool                        set_real(int _iRows, int _iCols, double _dblReal);

        /*zero or one set filler*/
        bool                        zero_set();
        bool                        one_set();

        /*Config management*/
        void                        whoAmI();
        bool                        isEmpty();

        std::wstring                toString(int _iPrecision, int _iLineLen);

        InternalType*               clone();
        bool                        append(int _iRows, int _iCols, Double *_poSource);
        bool                        fillFromCol(int _iCols, Double *_poSource);
        bool                        fillFromRow(int _iRows, Double *_poSource);

        bool                        operator==(const InternalType& it);
        bool                        operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring        getTypeStr() {return L"constant";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring        getShortTypeStr() {return L"s";}
    protected :
        RealType                    getType(void);

    private :
        double                      get_null_value();
        void                        parseSubMatrix(std::wostringstream& ostr, int* _piDims, int _iDims, int _iDim, int _iPrecision, int _iLineLen);
        void                        subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen);
        virtual Double*             create_empty(int _iDims, int* _piDims, bool _bComplex = false);

    };
}

#endif /* !__DOUBLE_HXX__ */
