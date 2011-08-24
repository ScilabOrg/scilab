/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __XMLOBJECTS_HXX__
#define __XMLOBJECTS_HXX__

#include <string>
#include <sstream>
#include <typeinfo>

extern "C" {
#include "xml_mlist.h"
}

namespace org_modules_xml
{
    class VariableScope;

    class XMLObject
    {

    public :
        XMLObject();
        virtual ~XMLObject() { }

        virtual const XMLObject * getXMLObjectParent() const { return 0; }
        virtual const std::string toString() const { return std::string(""); }
        virtual const std::string dump() const { return std::string(""); }

        int getId() const { return id; }
        int createOnStack(int pos) const;

        template <class T>
        static T * getFromId(int id) { return static_cast<T *>(getVariableFromId(id)); }

        static const std::string intToStr(int n)
            {
                std::stringstream oss;
                oss << n;
                return oss.str();
            }

    protected :
        int id;
        int scilabType;
	
        static VariableScope & scope;

    private :
	static XMLObject * getVariableFromId(int id);
    };
}

#endif
