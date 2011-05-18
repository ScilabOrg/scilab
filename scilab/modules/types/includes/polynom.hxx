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

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __POLYNOM_HXX__
#define __POLYNOM_HXX__

#include "arrayof.hxx"
#include "singlepoly.hxx"
#include "double.hxx"

namespace types
{
    class Polynom : public ArrayOf<SinglePoly*>
    {
    public :
                                Polynom();
                                Polynom(std::wstring _szVarName, int _iRows, int _iCols, int *_piRank);
                                Polynom(std::wstring _szVarName, int _iRows, int _iCols, const int *_piRank);
                                Polynom(std::wstring _szVarName, int _iDims, int* _piDims, const int *_piRank);
        virtual                 ~Polynom();

        // FIXME : Should not return NULL
        InternalType*           clone();

        bool                    setCoef(int _iRows, int _iCols, Double *_pdblCoef);
        bool                    setCoef(int _iIdx, Double *_pdblCoef);

	    virtual void            setComplex(bool _bComplex);

        virtual RealType        getType();
        bool                    isPoly() { return true; }
        void                    whoAmI(void);
        std::wstring            getVariableName();
        bool                    getRank(int *_piRank);
        int                     getMaxRank(void);
        Double*                 evaluate(Double* _pdblValue);
        void                    updateRank(void);
        Double*                 getCoef(void);
        void                    setCoef(Double *_pCoef);
        Double*                 extractCoef(int _iRank);
        bool                    insertCoef(int _iRank, Double* _pCoef);

        std::wstring            getRowString(int* _piDims, int _iDims, int _iPrecison, int _iLineLen, bool _bComplex);
        std::wstring            getColString(int* _piDims, int _iDims, int _iPrecison, int _iLineLen, bool _bComplex);
        std::wstring            getMatrixString(int* _piDims, int _iDims, int _iPrecison, int _iLineLen, bool _bComplex);


        bool                    operator==(const InternalType& it);
        bool                    operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring    getTypeStr() {return L"polynomial";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring    getShortTypeStr() {return L"p";}

    protected :
        std::wstring            m_szVarName;

    private :
        virtual void            subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen);

        virtual SinglePoly*     getNullValue();
        virtual Polynom*        createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
        virtual SinglePoly*     copyValue(SinglePoly* _pData);
        virtual void            deleteAll();
        virtual void            deleteImg();
        virtual SinglePoly**    allocData(int _iSize);

    };
}

#endif /* !__POLYNOM_HXX__ */
