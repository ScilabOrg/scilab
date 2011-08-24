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

#ifndef __XMLNODELIST_HXX__
#define __XMLNODELIST_HXX__

#include "XMLList.hxx"
#include "xml.h"

namespace org_modules_xml
{
    class XMLElement;
    class XMLDocument;

    class XMLNodeList : public XMLList
    {

        const XMLDocument & doc;
        xmlNode * parent;
        int prev;
        xmlNode * prevNode;

    public :
        XMLNodeList(const XMLDocument & doc, xmlNode * parent);
        ~XMLNodeList();

        const std::string dump() const;
        const XMLObject * getListElement(int index);
        void removeElementAtPosition(int index);
        void setElementAtPosition(double index, const XMLElement & elem);
        void setElementAtPosition(double index, const XMLDocument & document);
        void setElementAtPosition(double index, const std::string & xmlCode);
	void setElementAtPosition(double index, const XMLNodeList & list);
        xmlNode * getRealNode() const { return parent->children; }
        const XMLObject * getXMLObjectParent() const;

    protected :
        void replaceAtIndex(int index, const XMLElement & elem);
        void insertAtEnd(const XMLElement & elem);
        void insertAtBeginning(const XMLElement & elem);
        void insertAtIndex(int index, const XMLElement & elem);

    private :
        static int getNodeListSize(xmlNode * node);
        xmlNode * getListNode(int index);
    };
}

#endif
