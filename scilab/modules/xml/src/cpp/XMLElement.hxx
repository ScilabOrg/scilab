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

#ifndef __XMLELEMENT_HXX__
#define __XMLELEMENT_HXX__

#include <string>

#include "xml.h"

namespace org_modules_xml
{
    class XMLObject;
    class XMLDocument;
    class XMLNodeList;
    class XMLNs;
    class XMLAttr;

    class XMLElement : public XMLObject
    {
        xmlNode * node;
        const XMLDocument & doc;

    public:
        XMLElement(const XMLDocument & doc, xmlNode * node);
        XMLElement(const XMLDocument & doc, const char * name);
        ~XMLElement();

        const char * getNodeName(void) const { return (const char *)node->name; }
        void setNodeName(const std::string & name) const;
        const XMLNs * getNodeNameSpace() const;
        void setNodeNameSpace(const XMLNs & ns) const;
        const char * getNodeContent() const;
        void setNodeContent(const std::string & content) const;
        int getNodeType(void) const { return node->type; }
        const XMLAttr * getAttributes(void) const;
        void setAttributes(const XMLAttr & attrs) const;
        const XMLElement * getParentElement() const;
        const XMLNodeList * getChildren() const;
        void setChildren(const XMLElement & elem) const;
        void setChildren(const XMLNodeList & list) const;
        void setChildren(const std::string & xmlCode) const;
        void addNamespace(const XMLNs & ns) const;
        const XMLNs * getNamespaceByPrefix(const char * prefix) const;
        const XMLNs * getNamespaceByHref(const char * href) const;
        xmlNode * getRealNode() const { return node; }
        const XMLDocument & getXMLDocument() const { return doc; }
        const XMLObject * getXMLObjectParent() const;
        const std::string dump() const;
        const std::string toString() const;
    };
}

#endif
