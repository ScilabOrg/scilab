/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __XMLVALIDATION_HXX__
#define __XMLVALIDATION_HXX__

#include <cstdio>
#include <list>
#include <string>

#include "xml.h"

namespace org_modules_xml
{
class XMLObject;
class XMLDocument;

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Base class for the XML validation.
 */
class XMLValidation: public XMLObject
{

public:
    /**
     * Retrieve all the open validation files
     * @return a list containing the validation objects
     */
    static const std::list < XMLValidation * >&getOpenValidationFiles();

    /**
     * Close all the validation files
     */
    static void closeAllValidationFiles();

    /**
     * Default constructor
     */
    XMLValidation();

    /**
     * Validate an already parsed document
     * @param doc the document to validate
     * @param error a string which will contain error messages
     */
    virtual bool validate(const XMLDocument & doc, std::string * error) const = 0;

    /**
     * Validate a document accessed via a xmlTextReader
     * @param reader the reader to use
     * @param error a string which will contain error messages
     */
    virtual bool validate(xmlTextReader * reader, std::string * error) const = 0;

    /**
     * Validate a document with a given path
     * @param path the document path
     * @param error a string which will contain error messages
     */
    bool validate(const char *path, std::string * error) const;

    /**
     * Validate a document with a given content as string
     * @param xmlCode the document code
     * @param error a string which will contain error messages
     */
    bool validate(const std::string & xmlCode, std::string * error) const;

    /**
     * @return the validation file
     */
    template < typename T > T * getValidationFile() const
    {
        return (T *) validationFile;
    }

protected:
    static std::string * errorBuffer;
    static std::list < XMLValidation * >&openValidationFiles;

    /**
     * Error function for the XML parser
     * @see http://xmlsoft.org/html/libxml-xmlerror.html#xmlGenericErrorFunc
     */
    static void errorFunction(void *ctx, const char *msg, ...);

    static void errorReaderFunction(void * arg, const char * msg, xmlParserSeverities severity, xmlTextReaderLocatorPtr locator);

    void *validationFile;
};
}

#endif
