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


#ifndef __INTERNAL_HXX__
#define __INTERNAL_HXX__

#include <vector>
#include <iostream>
#include <string.h>
#include "anytype.hxx"


namespace types
{
  class InternalType
  {
  public :
      enum RealType {
          /* Internal Type */
          RealInternal,
          /* Generic Types */
          RealGeneric,
          RealInt,
          RealString,
          RealDouble,
	  RealSparse,
          RealBool,
          RealFloat,
          RealPoly,
          RealSinglePoly,
          /* Callable */
          RealFunction,
          RealMacro,
          RealMacroFile,
          /* Implicit List */
          RealImplicitList,
          /* Container */
          RealContainer,
          RealList,
          RealStruct,
          RealCell,
          /* User */
          RealUserType,
          /*For list operation*/
          RealListOperation, //parent type
          RealListInsertOperation,
          RealListDeleteOperation,
          RealListUndefinedOperation
      };

  protected :
                                        InternalType() : m_iRef(0), m_bAllowDelete(true) {}

  public :
      virtual                           ~InternalType(){};
      virtual void                      whoAmI(void)const { std::cout << "types::Inernal"; }

      virtual bool                      isAssignable(void)const { return false; }
      virtual RealType                  getType(void)const { return RealInternal; }


      virtual std::string	            toString(int _iPrecison, int _iLineLen)const = 0;
      virtual InternalType*             clone(void)const = 0;


      void IncreaseRef()
      {
          m_iRef++;
      }

      void DecreaseRef()
      {
          if(m_iRef > 0)
          {
              m_iRef--;
          }
      }

      bool	                            isDeletable()const { return m_iRef == 0; }
      bool	                            isRef(int _iRef = 0)const { return m_iRef > _iRef; }
      int                               getRef()const { return m_iRef; }

      /* return type as string ( double, int, cell, list, ... )*/
      virtual std::string               getTypeStr()const = 0;
      /* return type as short string ( s, i, ce, l, ... )*/
      virtual std::string               getShortTypeStr()const = 0;

      virtual bool                      operator==(const InternalType& it)const { return (getType() == (const_cast<InternalType *>(&it))->getType()); }
      virtual bool                      operator!=(const InternalType& it)const { return !(*this == it); }

      /**
      ** GenericType
      ** \{
      */

      /* GenericType */
      virtual GenericType*              getAsGenericType(void) { return NULL; }
      virtual GenericType const*              getAsGenericType(void)const { return NULL; }

      /* String */
      bool                              isString(void)const { return (getType() == RealString); }
      virtual String const*                   getAsString(void)const { return NULL; }
      virtual String*                   getAsString(void) { return NULL; }

      /* Double */
      bool                              isDouble(void)const { return (getType() == RealDouble); }
      virtual Double*                   getAsDouble(void) { return NULL; }
      virtual Double const*                   getAsDouble(void)const { return NULL; }

      /* Sparse */
      bool                              isSparse(void) const{ return (getType() == RealSparse); }
      virtual Sparse*                   getAsSparse(void) { return NULL; }
      virtual Sparse const*                   getAsSparse(void)const { return NULL; }

      /* Float */
      bool                              isFloat(void)const { return (getType() == RealFloat); }
      virtual Float*                    getAsFloat(void) { return NULL; }
      virtual Float const*              getAsFloat(void)const { return NULL; }

      /* Int */
      bool                              isInt(void) const{ return (getType() == RealInt); }
      virtual Int*                      getAsInt(void) { return NULL; }
      virtual Int const*                      getAsInt(void) const { return NULL; }

      /* Bool */
      bool                              isBool(void) const{ return (getType() == RealBool); }
      virtual Bool*                     getAsBool(void) { return NULL; }
      virtual Bool const*                     getAsBool(void)const { return NULL; }

      /* Poly Matrix */
      bool                              isPoly(void) { return (getType() == RealPoly); }
      virtual MatrixPoly*               getAsPoly(void) { return NULL; }
      virtual MatrixPoly const*               getAsPoly(void)const { return NULL; }

      /* Single Poly */
      bool                              isSinglePoly(void)const { return (getType() == RealSinglePoly); }
      virtual Poly*                     getAsSinglePoly(void) { return NULL; }
      virtual Poly const*                     getAsSinglePoly(void)const { return NULL; }

      /**
      ** \}
      */

      /**
      ** Callable
      ** \{
      */
      virtual bool                      isCallable(void) const{ return false; }
      virtual Callable  const*                 getAsCallable(void) const{ return NULL; }
      virtual Callable*                 getAsCallable(void) { return NULL; }

      /* Function */
      bool                              isFunction(void) { return (getType() == RealFunction); }
      virtual Function*                 getAsFunction(void) { return NULL; }
      virtual Function  const*                 getAsFunction(void)  const{ return NULL; }
      /* Macro */
      bool                              isMacro(void) { return (getType() == RealMacro); }
      virtual Macro*                    getAsMacro(void) { return NULL; }
      virtual Macro const*                    getAsMacro(void) const { return NULL; }


      /* MacroFile */
      bool                              isMacroFile(void) { return (getType() == RealMacroFile); }
      virtual MacroFile*                getAsMacroFile(void) { return NULL; }
      virtual MacroFile const*                getAsMacroFile(void)  const{ return NULL; }

      /**
      ** \}
      */

      /**
      ** Container
      ** \{
      */
      virtual bool                      isContainer(void) { return false; }
      virtual Container*                getAsContainer(void) { return NULL; }
      virtual Container const*                getAsContainer(void)  const{ return NULL; }

      /* List */
      bool                              isList(void) { return (getType() == RealList); }
      virtual List*                     getAsList(void) { return NULL; }
      virtual List const*                     getAsList(void)  const{ return NULL; }

      /* Struct */
      bool                              isStruct(void) { return (getType() == RealStruct); }
      virtual Struct*                   getAsStruct(void) { return NULL; }
      virtual Struct const*                   getAsStruct(void) const { return NULL; }

      /* Cell */
      bool                              isCell(void) { return (getType() == RealCell); }
      virtual Cell*                     getAsCell(void) { return NULL; }
      virtual Cell const*                     getAsCell(void) const { return NULL; }

      /**
      ** \}
      */

      /* ImplicitList */
      bool                              isImplicitList(void) { return (getType() == RealImplicitList); }
      virtual ImplicitList*             getAsImplicitList(void) { return NULL; }
      virtual ImplicitList const*             getAsImplicitList(void) const { return NULL; }


      /**
      ** List Operations
      ** \{
      */

      /* ListOperation */
      bool                              isListOperation(void) { return (getType() == RealListOperation); }
      virtual ListOperation*            getAsListOperation(void) { return NULL; }
      virtual ListOperation const*            getAsListOperation(void) const { return NULL; }

      /* ListDelete */
      bool                              isListDelete(void) { return (getType() == RealListDeleteOperation); }
      virtual ListDelete*               getAsListDelete(void) { return NULL; }
      virtual ListDelete const*               getAsListDelete(void) const { return NULL; }

      /* ListAdd */
      bool                              isListInsert(void) { return (getType() == RealListInsertOperation); }
      virtual ListInsert*               getAsListInsert(void) { return NULL; }
      virtual ListInsert const*               getAsListInsert(void) const { return NULL; }

      /**
      ** \}
      */

  private :
      int                               m_iRef;
      //use to know if we can delete this variables or if it's link to a scilab variable.
      bool                              m_bAllowDelete;
  };

  /*
  ** List of types
  */
  typedef std::vector<InternalType *>   typed_list;

}
#endif /* !__INTERNAL_HXX__ */
