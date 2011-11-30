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

extern "C"
{
#ifndef XML_XPATH_CHECKNS
#define XML_XPATH_CHECKNS
#endif
}

namespace org_modules_xml
{
    class XMLElement;
    class XMLObject;
    class XMLXPath;
    class XMLValidation;

    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Class to wrap a xmlDoc
     * @see http://xmlsoft.org/html/libxml-tree.html#xmlDoc
     */
    class XMLDocument : public XMLObject
    {
        static std::list<XMLDocument *> & openDocs;
        xmlDoc * document;

    public :

        /**
         * Gets the list of open docs
         * @return the list
         */
        static const std::list<XMLDocument *> & getOpenDocuments();

        /**
         * Closes all the open documents
         */
        static void closeAllDocuments();

        /**
         * Builds a document with a given path (can be an url)
         * @param path the document path
         * @param validate a boolean to indicate if the document must be validated in using a DTD
         * @param error a pointer to a string which will receive the error message
         */
        XMLDocument(const char * path, bool validate, std::string * error);

        /**
         * Builds a document with a given code
         * @param xmlCode the XML code
         * @param validate a boolean to indicate if the document must be validated in using a DTD
         * @param error a pointer to a string which will receive the error message
         */
        XMLDocument(const std::string & xmlCode, bool validate, std::string * error);

        /**
         * Builds a simple document
         * @param uri the document uri
         * @param version the xml version
         */
        XMLDocument(char * uri, char * version);

        ~XMLDocument();

        void * getRealXMLPointer() const;

        /**
         * @return the xmlDoc behind this XMLDocument
         */
        xmlDoc * getRealDocument() const { return document; }

        /**
         * @return the document root
         */
        const XMLElement * getRoot() const;

        /**
         * @param value the root to set
         */
        void setRoot(const XMLElement & value) const;

        /**
         * Replaces the root element by the the root of the xmlCode/
         * @param xmlCode the XML code
         * @param error a pointer to a string which will receive the error message
         */
        void setRoot(const std::string & xmlCode, std::string * error) const;

        /**
         * @return the document URL
         */
        const char * getDocumentURL() const;

        /**
         * @param value the document URL to set
         */
        void setDocumentURL(const std::string & value) const;

        /**
         * Makes an XPath query on the document
         * @param query the XPath query
         * @param namespaces an a matrix nx2 containing mapping between prefix and href
         * @param length the number of namespaces
         * @param error a pointer to a string which will receive the error message
         * @return a pointer on a XPath object
         */
        const XMLXPath * makeXPathQuery(const char * query, char ** namespaces, int length, std::string * error);

        const XMLObject * getXMLObjectParent() const;
        const std::string dump(bool indent) const;
        const std::string toString() const;

    private :

        /**
         * Error function for the XML parser
         * @see http://xmlsoft.org/html/libxml-xmlerror.html#xmlGenericErrorFunc
         */
        static void errorFunction(void * ctx, const char * msg, ...);

        /**
         * Error function which does nothing for the XML parser
         * @see http://xmlsoft.org/html/libxml-xmlerror.html#xmlGenericErrorFunc
         */
        static void errorFunctionWithoutOutput(void * ctx, const char * msg, ...) { }

        /**
         * Error function used when the XPath query is compiled/
         * @see http://xmlsoft.org/html/libxml-xmlerror.html#xmlStructuredErrorFunc
         */
        static void errorXPathFunction(void * ctx, xmlError * error);

        /**
         * Reads and parses a document given in a file.
         * @param filename the file name
         * @param validate a boolean to indicate if the document must be validated in using a DTD
         * @param error a string where to write the parsing errors
         * @return a pointer on a xmlDoc
         */
        static xmlDoc * readDocument(const char * filename, bool validate, std::string * error);

        /**
         * Read and parse a document given in a string.
         * @param xmlCode the XML code
         * @param validate a boolean to indicate if the document must be validated in using a DTD
         * @param error a string where to write the parsing errors
         * @return a pointer on a xmlDoc
         */
        static xmlDoc * readDocument(const std::string & xmlCode, bool validate, std::string * error);

        /**
         * Initializes the context
         * @param error a string where to write the parsing errors
         * @param validate a boolean to indicate if the document must be validated in using a DTD
         * @return a pointer on a context
         */
        static xmlParserCtxt * initContext(std::string * error, bool validate);

        static std::string * errorBuffer;
        static std::string * errorXPathBuffer;
    };
}

#endif
