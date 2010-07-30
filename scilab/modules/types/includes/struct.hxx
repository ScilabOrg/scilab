/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/


#ifndef __STRUCT_HXX__
#define __STRUCT_HXX__

#include <map>
#include "container.hxx"
#include "symbol.hxx"

namespace types
{
    class Struct : public Container
    {
    public :
                                                Struct();
                                                ~Struct();

    private :
                                                Struct(Struct const&_oListCopyMe);
        std::map<std::string, InternalType *>*  getData();
        std::map<std::string, InternalType *> const*  getData() const;

    public :
        int                                     size_get() const;

        void                                    whoAmI(void)const { std::cout << "types::Struct"; };

        RealType                                getType(void)const { return RealStruct; }

        /**
        ** add(Symbol *_psKey, InternalType *_typedValue)
        ** Append the given value to the struct
        */
        void                                    add(const std::string& _sKey, InternalType *_typedValue);

        /**
        ** add(Symbol *_psKey)
        ** Append an null value to the struct
        */
        void                                    add(const std::string& _sKey);

        /**
        ** get(Symbol *_psKey)
        ** Append the given value to the end of the List
        */
        InternalType*                           get(const std::string& _sKey);

        /**
        **
        */
        bool                                    exists(const std::string& _sKey);

        std::vector<InternalType*>              extract(list<string> _stFields);

        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        Struct*                                 clone() const;

        std::string                             toString(int _iPrecision, int _iLineLen) const;

        Struct*                                 getAsStruct(void) { return this; }
        Struct const*                                 getAsStruct(void)const { return this; }

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string                     getTypeStr() const{return string("struct");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string                     getShortTypeStr()const {return string("st");}
    private :
        std::map<std::string, InternalType *>*  m_plData;
    };
}

#endif /* !__STRUCT_HXX__ */
