/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MACRO_HXX__
#define __MACRO_HXX__

#include <list>
#include <string>
#include "context.hxx"
#include "types.hxx"
#include "callable.hxx"
#include "double.hxx"
#include "seqexp.hxx"

namespace types
{
class EXTERN_AST Macro : public Callable
{
public :
    Macro() : Callable(),
        m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargin"))),
        m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargout"))),
        m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargin"))),
        m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargout")))
    {
    }

    Macro(const std::wstring& _stName, std::list<symbol::Variable*> &_inputArgs, std::list<symbol::Variable*> &_outputArgs, ast::SeqExp &_body, const std::wstring& _stModule);
    virtual                     ~Macro();

    // FIXME : Should not return NULL;
    InternalType*               clone();

    inline ScilabType           getType(void)
    {
        return ScilabMacro;
    }
    inline ScilabId             getId(void)
    {
        return IdMacro;
    }
    bool                        isMacro()
    {
        return true;
    }

    void                        whoAmI();

    bool                        toString(std::wostringstream& ostr);

    Callable::ReturnValue       call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);

    inline void cleanCall(symbol::Context * pContext, int oldPromptMode);

    ast::SeqExp*                getBody();

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring        getTypeStr()
    {
        return L"function";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring        getShortTypeStr()
    {
        return L"function";
    }

    std::list<symbol::Variable*>*   inputs_get();
    std::list<symbol::Variable*>*   outputs_get();

    virtual int getNbInputArgument(void);
    virtual int getNbOutputArgument(void);

    bool operator==(const InternalType& it);

private :
    std::list<symbol::Variable*>*   m_inputArgs;
    std::list<symbol::Variable*>*   m_outputArgs;
    ast::SeqExp*                    m_body;
    bool                            bAutoAlloc;
    symbol::Variable*               m_Nargin;
    symbol::Variable*               m_Nargout;
    symbol::Variable*               m_Varargin;
    symbol::Variable*               m_Varargout;
    Double*                         m_pDblArgIn;
    Double*                         m_pDblArgOut;

};
}


#endif /* !__MACRO_HXX__ */
