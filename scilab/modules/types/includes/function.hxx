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
#include "callable.hxx"
#include "types.hxx"

#define MAX_OUTPUT_VARIABLE		64

namespace types
{
    class Function : public Callable
    {
    public :
        typedef ReturnValue (*GW_FUNC)(typed_list &in, int _iRetCount, typed_list &out);
        typedef int (*OLDGW_FUNC)(char *fname, int* _piKey);

                            Function(): Callable() {};
                            Function(std::string const&_szName, GW_FUNC _pFunc, std::string const&_szModule);
                            ~Function();

        //FIXME : Should not return NULL
        Function*           clone() const { return NULL; }

        static Function*    createFunction(std::string const& _szName, GW_FUNC _pFunc, std::string const& _szModule);
        static Function*    createFunction(std::string const& _szName, OLDGW_FUNC _pFunc, std::string const& _szModule);

        Function*           getAsFunction(void);
        Function const*           getAsFunction(void)const;
        RealType            getType(void)const { return RealFunction; }

        void                whoAmI()const;

        std::string         toString(int _iPrecision, int _iLineLen)const;

        virtual ReturnValue call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string getTypeStr()const {return string("fptr");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string getShortTypeStr()const {return string("fptr");}
    private :
        GW_FUNC             m_pFunc;
    };

    class WrapFunction : public Function
    {
    public :
                            WrapFunction(std::string const&_szName, OLDGW_FUNC _pFunc, std::string const&_szModule);

                            Callable::ReturnValue call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);
    private :
        OLDGW_FUNC          m_pOldFunc;
        InternalType*       m_pTempOut[MAX_OUTPUT_VARIABLE];
    };

    class GatewayStruct
    {
    public :
        typed_list*         m_pIn;
        InternalType**      m_pOut;
        int*                m_piRetCount;
        char*               m_pstName;
        int*                m_pOutOrder;

                            GatewayStruct(){};
                            ~GatewayStruct(){};
    };
}


#endif /* !__FUNCTION_HXX__ */
