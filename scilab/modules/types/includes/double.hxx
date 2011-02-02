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

// This code is separated in double.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

#ifndef __ARRAYOF_HXX__
    #error This file must only be include by arrayof.hxx
#endif

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
        double*                     getReal() const;
        double                      getReal(int _iRows, int _iCols);
        bool                        setReal(double *_pdblReal);
        bool                        setReal(const double *_pdblReal);
        bool                        setReal(int* _piReal); //to translate int to double matrix
        bool                        setReal(int _iPos, double _dblReal);
        bool                        setReal(int _iRows, int _iCols, double _dblReal);

        /*zero or one set filler*/
        bool                        setZeros();
        bool                        setOnes();

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
        double                      getNullValue();
        void                        parseSubMatrix(std::wostringstream& ostr, int* _piDims, int _iDims, int _iDim, int _iPrecision, int _iLineLen);
        void                        subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen);
        virtual Double*             createEmpty(int _iDims, int* _piDims, bool _bComplex = false);

    };
}
