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

#ifndef __FUNCTION_HXX__
#define __FUNCTION_HXX__


#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

#include <string>
#include "types.hxx"
#include "callable.hxx"

#define MAX_OUTPUT_VARIABLE		64

#define CPP_GATEWAY_PROTOTYPE(__gateway__) types::Function::ReturnValue __gateway__(types::typed_list &in, int _iRetCount, types::typed_list &out)

namespace types
{
    class Function : public Callable
    {
    public :
        typedef ReturnValue (*GW_FUNC)(typed_list &in, int _iRetCount, typed_list &out);
        typedef int (*OLDGW_FUNC)(char *fname, int* _piKey);

                                Function() : Callable() {};
                                Function(std::wstring _szName, GW_FUNC _pFunc, std::wstring _szModule);
                                ~Function();

        //FIXME : Should not return NULL
        InternalType*           clone();

        static Function*        createFunction(std::wstring _szName, GW_FUNC _pFunc, std::wstring _szModule);
        static Function*        createFunction(std::wstring _szName, OLDGW_FUNC _pFunc, std::wstring _szModule);

        Function*               getAsFunction(void);
        RealType                getType(void) { return RealFunction; }
        bool                    isFunction() { return true; }

        void                    whoAmI();

        std::wstring            toString(int _iPrecision, int _iLineLen);

        virtual ReturnValue     call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring    getTypeStr() {return L"fptr";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring    getShortTypeStr() {return L"fptr";}

        GW_FUNC                 getFunc() { return m_pFunc; }
    private :
        GW_FUNC                 m_pFunc;
    };

    class WrapFunction : public Function
    {
    private :
                                WrapFunction(WrapFunction* _pWrapFunction);
    public :
                                WrapFunction(std::wstring _szName, OLDGW_FUNC _pFunc, std::wstring _szModule);

                                Callable::ReturnValue call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);
        InternalType*           clone();

        OLDGW_FUNC              getFunc() { return m_pOldFunc; }

    private :
        OLDGW_FUNC              m_pOldFunc;
        InternalType*           m_pTempOut[MAX_OUTPUT_VARIABLE];
    };

    class GatewayStruct
    {
    public :
        typed_list*             m_pIn;
        InternalType**          m_pOut;
        int*                    m_piRetCount;
        wchar_t*                m_pstName;
        int*                    m_pOutOrder;

                                GatewayStruct(){};
                                ~GatewayStruct(){};
    };
}


#endif /* !__FUNCTION_HXX__ */
