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

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLXPath.hxx"
#include "VariableScope.hxx"

extern "C" {
#include "expandPathVariable.h"
#include "MALLOC.h"
}

#define BUFFER_SIZE 1024

namespace org_modules_xml
{

    std::string * XMLDocument::errorBuffer = 0;
    std::string * XMLDocument::errorXPathBuffer = 0;
    std::list<XMLDocument *> & XMLDocument::openDocs = *new std::list<XMLDocument *>();

    /**
     * Default constructor
     * @param path the path to the file (can be an url)
     * @param error will contain parsing errors if exists
     */
    XMLDocument::XMLDocument(const char * path, char ** error) : XMLObject()
    {
        char * expandedPath = expandPathVariable(const_cast<char *>(path));
        document = readDocument(const_cast<const char *>(expandedPath), error);
        FREE(expandedPath);
        if (document)
        {
            openDocs.push_back(this);
        }
        scope.registerPointers(document, this);
        scilabType = XMLDOCUMENT;
    }

    /**
     * Default constructor
     * @param path the path to the file (can be an url)
     * @param error will contain parsing errors if exists
     */
    XMLDocument::XMLDocument(const std::string & xmlCode, char ** error) : XMLObject()
    {
        document = readDocument(xmlCode, error);
        if (document)
        {
            openDocs.push_back(this);
        }
        scope.registerPointers(document, this);
        scilabType = XMLDOCUMENT;
    }

    XMLDocument::XMLDocument(char * uri, char * version)
    {
        char * newUri = 0;
        if (!version)
        {
            version = const_cast<char *>("1.0");
        }
        document = xmlNewDoc((xmlChar *)version);
        openDocs.push_back(this);
        scope.registerPointers(document, this);
        scilabType = XMLDOCUMENT;

        if (uri && strlen(uri))
        {
            newUri = (char *)xmlMalloc(sizeof(char *) * (strlen(uri) + 1));
            memcpy(newUri, uri, sizeof(char) * (strlen(uri) + 1));
            document->URL = (xmlChar *)newUri;
        }
    }

    /**
     * Destructor
     */
    XMLDocument::~XMLDocument()
    {
        scope.unregisterPointer(document);
        scope.removeId<XMLDocument>(id);
        if (document)
        {
            openDocs.remove(this);
            xmlFreeDoc(document);
        }
        if (errorBuffer)
        {
            delete errorBuffer;
            errorBuffer = 0;
        }
        if (errorXPathBuffer)
        {
            delete errorXPathBuffer;
            errorXPathBuffer = 0;
        }
    }

    const XMLXPath * XMLDocument::makeXPathQuery(const char * query, char ** error)
    {
        if (errorXPathBuffer)
        {
            delete errorXPathBuffer;
        }
        errorXPathBuffer = new std::string();

        xmlXPathContext * ctxt = xmlXPathNewContext(document);
        if (!ctxt)
        {
            *error = const_cast<char *>("Cannot create a parser context");
            return 0;
        }

        xmlSetStructuredErrorFunc(ctxt, XMLDocument::errorXPathFunction);
        xmlXPathCompExpr * expr = xmlXPathCtxtCompile(ctxt, (const xmlChar *)query);
        if (!expr)
        {
            *error = const_cast<char *>(errorXPathBuffer->c_str());
            return 0;
        }

        xmlXPathObject * xpath = xmlXPathCompiledEval(expr, ctxt);
        if (!xpath)
        {
            *error = const_cast<char *>(errorXPathBuffer->c_str());
            return 0;
        }

        return new XMLXPath(*this, xpath);
    }

    const XMLObject * XMLDocument::getXMLObjectParent() const
    {
        return 0;
    }

    const std::string XMLDocument::toString() const
    {
        std::string str = "XML Document\n";
        str += "URL: " + std::string(getDocumentURL());

        return str;
    }

    const std::string XMLDocument::dump() const
    {
        xmlChar * buffer = 0;
        int size = 0;
        xmlDocDumpFormatMemory(document, &buffer, &size, 1);
        std::string str = std::string((const char *)buffer);
        xmlFree(buffer);

        return str;
    }

    /**
     * @return the root element of this XMLDocument
     */
    const XMLElement * XMLDocument::getRoot() const
    {
        return new XMLElement(*this, xmlDocGetRootElement(document));
    }

    void XMLDocument::setRoot(const XMLElement & elem) const
    {
        xmlNode * root = xmlDocGetRootElement(document);
        if (root != elem.getRealNode())
        {
            xmlNode * cpy = xmlCopyNodeList(elem.getRealNode());
            xmlUnlinkNode(cpy);
            xmlDocSetRootElement(document, cpy);
        }
    }

    void XMLDocument::setRoot(const std::string & xmlCode, char ** error) const
    {
        XMLDocument doc = XMLDocument(xmlCode, error);

        if (!*error)
        {
            setRoot(*doc.getRoot());
        }

        delete &xmlCode;
    }

    const char * XMLDocument::getDocumentURL() const
    {
        if (document->URL)
        {
            return (const char *)document->URL;
        }
        else
        {
            return "Undefined";
        }
    }

    void XMLDocument::setDocumentURL(const std::string & url) const
    {
        const char * oldURL = 0;
        char * newURL = 0;
        oldURL = url.c_str();

        xmlFree((void *)document->URL);
        newURL = (char *)xmlMalloc(sizeof(char *) * (strlen(oldURL) + 1));
        memcpy(newURL, oldURL, sizeof(char) * (strlen(oldURL) + 1));

        document->URL = (xmlChar *)newURL;
    }

    std::list<XMLDocument *> & XMLDocument::getOpenDocuments()
    {
        return openDocs;
    }

    void XMLDocument::closeAllDocuments()
    {
        int size = openDocs.size();
        XMLDocument * arr[size];
        int j = 0;
        for (std::list<XMLDocument *>::iterator i = openDocs.begin(); i != openDocs.end(); i++, j++)
        {
            arr[j] = *i;
        }
        for (j = 0; j < size; j++)
        {
            delete arr[j];
        }
    }

    /**
     * Read and parse a document
     * @param filename the file name
     * @param error a string where to write the parsing errors
     * @return a pointer on a xmlDoc
     */
    xmlDoc * XMLDocument::readDocument(const char * filename, char ** error)
    {
        xmlParserCtxt * ctxt = initContext(error);
        xmlDoc * doc;

        if (!ctxt)
        {
            return 0;
        }

        doc = xmlCtxtReadFile(ctxt, filename, 0, 0);
        if (!doc)
        {
            *error = const_cast<char *>(errorBuffer->c_str());
        }

        return doc;
    }

    /**
     * Read and parse a document
     * @param filename the file name
     * @param error a string where to write the parsing errors
     * @return a pointer on a xmlDoc
     */
    xmlDoc * XMLDocument::readDocument(const std::string & xmlCode, char ** error)
    {
        xmlParserCtxt * ctxt = initContext(error);
        xmlDoc * doc;

        if (!ctxt)
        {
            return 0;
        }

        doc = xmlCtxtReadDoc(ctxt, (const xmlChar *)xmlCode.c_str(), 0, 0, 0);
        if (!doc)
        {
            *error = const_cast<char *>(errorBuffer->c_str());
        }

        return doc;
    }

    xmlParserCtxt * XMLDocument::initContext(char ** error)
    {
        xmlParserCtxt * ctxt;

        if (errorBuffer)
        {
            delete errorBuffer;
        }
        errorBuffer = new std::string();

        ctxt = xmlNewParserCtxt();
        if (!ctxt)
        {
            *error = const_cast<char *>("Cannot create a parser context");
            return 0;
        }

        xmlSetGenericErrorFunc(ctxt, XMLDocument::errorFunction);

        return ctxt;
    }

    /**
     * Error function to use in the parser (see xmlSetGenericErrorFunc)
     * @param ctx the context
     * @param msg the error message
     * @param ... the arguments to format the error message
     */
    void XMLDocument::errorFunction(void * ctx, const char * msg, ...)
    {
        char str[BUFFER_SIZE];
        va_list args;

        va_start(args, msg);
        std::vsnprintf(str, BUFFER_SIZE, msg, args);
        va_end(args);
        errorBuffer->append(str);
    }

    void XMLDocument::errorXPathFunction(void * ctx, xmlError * error)
    {
        errorXPathBuffer->append(error->message);
    }
}
