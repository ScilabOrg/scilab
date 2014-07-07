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

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __DOUBLE_HXX__
#define __DOUBLE_HXX__

#include "arrayof.hxx"
#include "bool.hxx"
#include "dynlib_types.h"
#include "types_transposition.hxx"

namespace types
{
class TYPES_IMPEXP Double : public ArrayOf<double>
{
public :
    virtual						~Double();

    Double(double _dblReal);
    Double(double _dblReal, double _dblImg);
    Double(int _iRows, int _iCols, bool _bComplex = false, bool _bZComplex = false);
    Double(int _iRows, int _iCols, double **_pdblReal);
    Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);
    Double(int _iDims, int* _piDims, bool _bComplex = false, bool _bZComplex = false);

    static Double*              Empty();
    static Double*              Identity(int _iRows, int _iCols);
    static Double*              Identity(int _iDims, int* _piDims);


    /*data management*/
    double*                     getReal() const;
    double                      getReal(int _iRows, int _iCols);
    bool                        setInt(int* _piReal); //to translate int to double matrix

    /*zero or one set filler*/
    bool                        setZeros();
    bool                        setOnes();

    /*Config management*/
    void                        whoAmI();
    bool                        isEmpty();

    InternalType*               clone();
    bool                        fillFromCol(int _iCols, Double *_poSource);
    bool                        fillFromRow(int _iRows, Double *_poSource);
    bool                        append(int _iRows, int _iCols, InternalType* _poSource);

    //bool                        append(int _iRows, int _iCols, Double *_poSource);

    bool                        operator==(const InternalType& it);
    bool                        operator!=(const InternalType& it);

    bool                        isDouble()
    {
        return true;
    }


    bool isTrue()
    {
        if (isEmpty() || isComplex())
        {
            return false;
        }

        return type_traits::isTrue<double>(m_iSize, m_pRealData);
    }

    bool neg(InternalType *& out)
    {
        if (isEmpty())
        {
            out = this;
            return true;
        }

        return ArrayOf<double>::neg(out);
    }

    void                        setViewAsInteger(bool _bViewAsInteger = true)
    {
        m_bViewAsInteger = _bViewAsInteger;
    }
    bool                        isViewAsInteger()
    {
        return m_bViewAsInteger;
    }

    void                        convertToInteger();
    void                        convertFromInteger();

    void                        setViewAsZComplex(bool _bViewAsZComplex = true)
    {
        m_bViewAsZComplex = _bViewAsZComplex;
    }
    bool                        isViewAsZComplex()
    {
        return m_bViewAsZComplex;
    }

    void                        convertToZComplex();
    void                        convertFromZComplex();

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring        getTypeStr()
    {
        return L"constant";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring        getShortTypeStr()
    {
        return L"s";
    }

    inline ScilabType           getType(void)
    {
        return ScilabDouble;
    }
    inline ScilabId             getId(void)
    {
        return isIdentity() ? isComplex() ? IdIdentityComplex : IdIdentity
       : isEmpty() ? IdEmpty
               : isComplex() ? isScalar() ? IdScalarDoubleComplex
               : IdDoubleComplex
       : isScalar() ? IdScalarDouble
               : IdDouble;
    }

    virtual bool invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, ast::ConstVisitor & execFunc, const ast::CallExp & e)
    {
        if (isEmpty())
        {
            out.push_back(this);

            return true;
        }

        return ArrayOf<double>::invoke(in, opt, _iRetCount, out, execFunc, e);
    }

    inline bool conjugate(InternalType *& out)
    {
        if (isEmpty() || isIdentity() || !isComplex())
        {
            out = clone();
            return true;
        }

        if (isScalar())
        {
            out = new Double(m_pRealData[0], -m_pImgData[0]);
            return true;
        }

        if (m_iDims == 2)
        {
            Double * pReturn = new Double(getCols(), getRows(), true);
            out = pReturn;

            Transposition::conjugate(getSize(), m_pRealData, pReturn->m_pRealData, m_pImgData, pReturn->m_pImgData);
            return true;
        }

        return false;

    }

    virtual bool adjoint(InternalType *& out)
    {
        if (isEmpty())
        {
            out = this;
            return true;
        }

        if (isIdentity())
        {
            out = clone();
            return true;
        }

        if (isScalar())
        {
            if (m_bComplex)
            {
                out = new Double(m_pRealData[0], -m_pImgData[0]);
            }
            else
            {
                out = clone();
            }

            return true;
        }

        if (m_iDims == 2)
        {
            Double * pReturn = new Double(getCols(), getRows(), m_bComplex);
            out = pReturn;
            if (m_bComplex)
            {
                Transposition::adjoint(getRows(), getCols(), m_pRealData, pReturn->m_pRealData, m_pImgData, pReturn->m_pImgData);
            }
            else
            {
                Transposition::adjoint(getRows(), getCols(), m_pRealData, pReturn->m_pRealData);
            }

            return true;
        }

        return false;
    }

    virtual bool transpose(InternalType *& out)
    {
        if (isEmpty())
        {
            out = this;
            return true;
        }

        if (isIdentity() || isScalar())
        {
            out = clone();
            return true;
        }

        if (m_iDims == 2)
        {
            Double * pReturn = new Double(getCols(), getRows(), m_bComplex);
            out = pReturn;
            if (m_bComplex)
            {
                Transposition::transpose(getRows(), getCols(), m_pRealData, pReturn->m_pRealData, m_pImgData, pReturn->m_pImgData);
            }
            else
            {
                Transposition::transpose(getRows(), getCols(), m_pRealData, pReturn->m_pRealData);
            }

            return true;
        }

        return false;
    }

private :
    virtual bool                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);

    virtual double              getNullValue();
    virtual Double*             createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
    virtual double              copyValue(double _dblData);
    virtual void                deleteAll();
    virtual void                deleteImg();
    virtual double*             allocData(int _iSize);

    bool                        m_bViewAsInteger;
    bool                        m_bViewAsZComplex;

};
}

#ifdef _MSC_VER
template class TYPES_IMPEXP types::ArrayOf<double>; //Double
#endif
#endif /* !__DOUBLE_HXX__ */
