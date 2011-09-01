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

#ifndef __XMLVALIDATIONDTD_HXX__
#define __XMLVALIDATIONDTD_HXX__

#include "xml.h"

namespace org_modules_xml
{

    class XMLValidation;
    class XMLDocument;

    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Base class for the XML objects.
     */
    class XMLValidationDTD : public XMLValidation
    {

    public :

        /**
         * Default constructor
         */
        XMLValidationDTD(const char * path, char ** error);

        ~XMLValidationDTD();

	bool validate(const XMLDocument & doc, char ** error) const;

	const std::string toString() const;
    };
}

#endif
