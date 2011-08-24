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

#include <string>

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLNodeList.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{

    XMLNodeList::XMLNodeList(const XMLDocument & _doc, xmlNode * _parent) : XMLList(), doc(_doc)
    {
        parent = _parent;
        size = getNodeListSize(parent->children);
        prev = 1;
        prevNode = parent->children;
        scope.registerPointers(parent->children, this);
    }

    XMLNodeList::~XMLNodeList()
    {
        scope.unregisterPointer(parent->children);
        scope.removeId<XMLNodeList>(id);
    }

    const XMLObject * XMLNodeList::getXMLObjectParent() const
    {
        return &doc;
    }

    const std::string XMLNodeList::dump() const
    {
        xmlBufferPtr buffer = xmlBufferCreate();
        for (xmlNode * cur = parent->children; cur; cur = cur->next)
        {
            xmlNodeDump(buffer, doc.getRealDocument(), cur, 0, 1);
            xmlBufferAdd(buffer, (xmlChar *)"\n", strlen("\n"));
        }
        std::string str = std::string((const char *)buffer->content);
        xmlBufferFree(buffer);

        return str;
    }

    const XMLObject * XMLNodeList::getListElement(int index)
    {
        xmlNode * n = getListNode(index);
        if (n)
        {
            return new XMLElement(doc, n);
        }

        return 0;
    }

    void XMLNodeList::removeElementAtPosition(int index)
    {
        if (size && index >= 1 && index <= size)
        {
            if (index == 1)
            {
                xmlNode * n = parent->children;
                scope.unregisterPointer(n);
                xmlUnlinkNode(n);
                xmlFreeNode(n);
                size--;
                if (size == 0)
                {
                    parent->children = 0;
                }
                prevNode = parent->children;
                prev = 1;
            }
            else
            {
                xmlNode * n = getListNode(index);
                if (n)
                {
                    xmlNode * next = n->next;
                    prevNode = prevNode->prev;
                    prev--;
                    xmlUnlinkNode(n);
                    xmlFreeNode(n);
                    prevNode->next = next;
                    size--;
                }
            }
        }
    }

    void XMLNodeList::setElementAtPosition(double index, const XMLElement & elem)
    {
        if (size == 0)
        {
            insertAtEnd(elem);
            prevNode = parent->children;
            prev = 1;
        }
        else if (index < 1)
        {
            insertAtBeginning(elem);
        }
        else if (index > size)
        {
            insertAtEnd(elem);
        }
        else if ((int)index == index)
        {
            replaceAtIndex((int)index, elem);
        }
        else
        {
            insertAtIndex((int)index, elem);
        }
    }

    void XMLNodeList::setElementAtPosition(double index, const XMLDocument & document)
    {
        const XMLElement * e = document.getRoot();
        setElementAtPosition(index, *e);
        delete e;
    }

    void XMLNodeList::setElementAtPosition(double index, const std::string & xmlCode)
    {
        char * error = 0;
        XMLDocument document = XMLDocument(xmlCode, &error);

        if (!error)
        {
            setElementAtPosition(index, document);
        }
        else
        {
            xmlNode * n = xmlNewText((xmlChar *)xmlCode.c_str());
            setElementAtPosition(index, XMLElement(doc, n));
        }

        delete &xmlCode;
    }

    void XMLNodeList::setElementAtPosition(double index, const XMLNodeList & list)
    {
        if (list.getSize() && list.getRealNode() != parent)
        {
            int pos = index;

            if (index < 1)
            {
                pos = 1;
            }
            else if (index > size)
            {
                pos = size++;
            }
            else if ((int)index != index)
            {
                pos++;
            }

            setElementAtPosition(index, XMLElement(doc, list.getRealNode()));
            for (xmlNode * cur = list.getRealNode()->next; cur; cur = cur->next)
            {
                setElementAtPosition((double)(pos++) + 0.5d, XMLElement(doc, cur));
            }
        }
    }

    void XMLNodeList::replaceAtIndex(int index, const XMLElement & elem)
    {
        xmlNode * n = getListNode(index);
        if (n)
        {
            if (index == 1)
            {
                scope.unregisterPointer(parent->children);
            }
            xmlNode * previous = n->prev;
            xmlNode * next = n->next;
            xmlNode * cpy = xmlCopyNode(elem.getRealNode(), 1);
            xmlUnlinkNode(cpy);
            xmlReplaceNode(n, cpy);
            xmlFreeNode(n);
            prevNode = cpy;
            cpy->prev = previous;
            cpy->next = next;
            if (index == 1)
            {
                scope.registerPointers(parent->children, this);
            }
        }
    }

    void XMLNodeList::insertAtEnd(const XMLElement & elem)
    {
        xmlNode * cpy = xmlCopyNode(elem.getRealNode(), 1);
        xmlUnlinkNode(cpy);
        xmlAddChild(parent, cpy);
        size++;
    }

    void XMLNodeList::insertAtBeginning(const XMLElement & elem)
    {
        xmlNode * cpy = xmlCopyNode(elem.getRealNode(), 1);
        xmlUnlinkNode(cpy);
        scope.unregisterPointer(parent->children);
        xmlAddPrevSibling(parent->children, cpy);
        scope.registerPointers(parent->children, this);
        size++;
    }

    void XMLNodeList::insertAtIndex(int index, const XMLElement & elem)
    {
        xmlNode * n = getListNode(index);
        if (n)
        {
            xmlNode * cpy = xmlCopyNode(elem.getRealNode(), 1);
            xmlUnlinkNode(cpy);
            xmlAddNextSibling(n, cpy);
            size++;
        }
    }

    xmlNode * XMLNodeList::getListNode(int index)
    {
        return XMLList::getListElement<xmlNode>(index, &prev, &prevNode);
    }

    inline int XMLNodeList::getNodeListSize(xmlNode * node)
    {
        int i = 0;
        for (xmlNode * n = node; n; n = n->next, i++);

        return i;
    }
}
