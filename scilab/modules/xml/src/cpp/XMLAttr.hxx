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

#ifndef __XMLATTR_HXX__
#define __XMLATTR_HXX__

#include <string>

#include "xml.h"

namespace org_modules_xml
{
    class XMLElement;
    class XMLObject;

    class XMLAttr : public XMLObject
    {
        const XMLElement & elem;

    public :
        XMLAttr(const XMLElement & elem);
        ~XMLAttr();

	int getSize() const;
	
        const char * getAttributeValue(const char * name) const;
        const char * getAttributeValue(const char * prefix, const char * name) const;
	void setAttributeValue(const char * prefix, const char * name, const char * value);
        void setAttributeValue(const char * name, const char * value);
        const XMLObject * getXMLObjectParent() const;
        const XMLElement & getElement() const { return elem; }
        const std::string toString() const;
    };
}

#endif
