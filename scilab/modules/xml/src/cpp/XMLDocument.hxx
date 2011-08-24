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

#ifndef __XMLDOCUMENT_HXX__
#define __XMLDOCUMENT_HXX__

#include <cstdio>
#include <list>
#include <cstring>
#include <string>

#include "xml.h"

namespace org_modules_xml
{
    class XMLElement;
    class XMLObject;
    class XMLXPath;

    class XMLDocument : public XMLObject
    {
        static std::list<XMLDocument *> & openDocs;
        xmlDoc * document;

    public :
        XMLDocument(const char * path, char ** error);
        XMLDocument(const std::string & xmlCode, char ** error);
        XMLDocument(char * uri, char * version);
        ~XMLDocument();

        static std::list<XMLDocument *> & getOpenDocuments();
        static void closeAllDocuments();

        xmlDoc * getRealDocument() const { return document; }
        const XMLElement * getRoot() const;
        void setRoot(const XMLElement & value) const;
        void setRoot(const std::string & xmlCode, char ** error) const;
        const XMLXPath * makeXPathQuery(const char * query, char ** error);
        const char * getDocumentURL() const;
        void setDocumentURL(const std::string & value) const;
        const XMLObject * getXMLObjectParent() const;
        const std::string dump() const;
        const std::string toString() const;

    private :
        static void errorFunction(void * ctx, const char * msg, ...);
        static void errorXPathFunction(void * ctx, xmlError * error);
        static xmlDoc * readDocument(const char * filename, char ** error);
        static xmlDoc * readDocument(const std::string & xmlCode, char ** error);
        static xmlParserCtxt * initContext(char ** error);

        static std::string * errorBuffer;
        static std::string * errorXPathBuffer;
    };
}

#endif
