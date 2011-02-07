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

#ifndef __CALLABLE_HXX__
#define __CALLABLE_HXX__

#include <string>
#include "types.hxx"

#include "visitor.hxx"

using namespace ast;
namespace types
{
  class Callable : public InternalType
  {
  public :
      enum ReturnValue
      {
          OK,
          OK_NoResult,
          Error
      };
    
                            Callable(): InternalType() {}
      virtual               ~Callable() {}

      Callable*             getAsCallable() { return this; }
      bool                  isCallable() { return true; }

      virtual ReturnValue   call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc) = 0;

      void                  setName(std::wstring _stName) { m_stName = _stName; }
      std::wstring          getName() { return m_stName; }
      void                  setModule(std::wstring _stModule) { m_stModule = _stModule; }
      std::wstring          getModule() { return m_stModule; }

      /* return type as string ( double, int, cell, list, ... )*/
      virtual std::wstring  getTypeStr() {return L"callable";}
      /* return type as short string ( s, i, ce, l, ... )*/
      virtual std::wstring  getShortTypeStr() = 0;
      virtual InternalType* clone(void) = 0;

  protected :
      std::wstring           m_stName;
      std::wstring           m_stModule;
  };
}


#endif /* !__CALLABLE_HXX__ */
