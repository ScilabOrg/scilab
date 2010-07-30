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
#include "types.hxx"
#include "callable.hxx"
#include "symbol.hxx"
#include "seqexp.hxx"

namespace types
{
    class Macro : public Callable
    {
    public :
                                    Macro(): Callable(){};
                                    Macro(const std::string& _stName, std::list<std::string> &_inputArgs, std::list<std::string> &_outputArgs, ast::SeqExp &_body, const string& _stModule);
        virtual                     ~Macro();

        // FIXME : Should not return NULL;
        Macro*                      clone() const{ return NULL; }

        Macro*                      getAsMacro(void);
        Macro const*                      getAsMacro(void)const;
        RealType                    getType(void) const;

        void                        whoAmI() const;

        std::string                 toString(int _iPrecision, int _iLineLen) const;

        Callable::ReturnValue       call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);

        ast::SeqExp*                body_get();

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string         getTypeStr()  const{return string("macro");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string         getShortTypeStr()  const{return string("function");}

    private :
        std::list<std::string>*     m_inputArgs;
        std::list<std::string>*     m_outputArgs;
        ast::SeqExp*                m_body;
    };
}


#endif /* !__MACRO_HXX__ */
