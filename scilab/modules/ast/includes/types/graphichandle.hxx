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

#ifndef __GRAPHICHANDLE_HXX__
#define __GRAPHICHANDLE_HXX__

#include "arrayof.hxx"
#include "bool.hxx"

namespace types
{
class EXTERN_AST GraphicHandle : public ArrayOf<long long>
{
public :
    GraphicHandle(long long _handle);
    GraphicHandle(int _iRows, int _iCols);
    GraphicHandle(int _iDims, int* _piDims);

    virtual                 ~GraphicHandle();

    InternalType*           clone();

    void                    whoAmI();

    bool                    isHandle()
    {
        return true;
    }

    bool                    isContainer()
    {
        return true;
    }

    bool                    operator==(const InternalType& it);
    bool                    operator!=(const InternalType& it);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring     getTypeStr()
    {
        return L"handle";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring     getShortTypeStr()
    {
        return L"h";
    }

    bool                    hasToString()
    {
        return false;
    }

    bool isTrue()
    {
        return false;
    }

    virtual bool neg(InternalType *& out)
    {
        return false;
    }

    virtual bool transpose(InternalType *& out)
    {
        return type_traits::transpose(*this, out);
    }

    virtual bool isFieldExtractionOverloadable() const
    {
        return true;
    }

    virtual bool invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, ast::ConstVisitor & execFunc, const ast::CallExp & e);

protected :
    inline ScilabType		getType(void)
    {
        return ScilabHandle;
    }
    inline ScilabId         getId(void)
    {
        return isScalar() ? IdScalarHandle : IdHandle;
    }

private :
    virtual bool            subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);

    virtual long long       getNullValue();
    virtual GraphicHandle*  createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
    virtual long long       copyValue(long long _handle);
    virtual void            deleteAll();
    virtual void            deleteImg();
    virtual long long*      allocData(int _iSize);
};
}

#ifdef _MSC_VER
template class types::ArrayOf<long long>; //GraphicHandle
#endif
#endif /* !__GRAPHICHANDLE_HXX__ */
