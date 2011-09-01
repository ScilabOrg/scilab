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
     * Base class for the XML objects.
     */
    class XMLValidation : public XMLObject
    {

    public :

        static const std::list<XMLValidation *> & getOpenValidationFiles();
	static void closeAllValidationFiles();

        /**
         * Default constructor
         */
        XMLValidation();

        virtual bool validate(const XMLDocument & doc, char ** error) const = 0;

        template <typename T>
        T * getValidationFile() const { return (T *)validationFile; }

    protected :

        static std::string * errorBuffer;
        static std::list<XMLValidation *> & openValidationFiles;

        /**
         * Error function for the XML parser
         * @see http://xmlsoft.org/html/libxml-xmlerror.html#xmlGenericErrorFunc
         */
        static void errorFunction(void * ctx, const char * msg, ...);

        void * validationFile;
    };
}

#endif
