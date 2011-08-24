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

extern "C"
{
#include "xml_mlist.h"
}

namespace org_modules_xml
{

    class XMLDocument;

    class XMLValidation
    {

    public :
	static bool validate(XMLDocument * doc, const char * method, const char * validationFile, char ** error);

    private :
        static bool validateWithDTD(XMLDocument * doc, const char * validationFile, char ** error);
        static bool validateWithSchema(XMLDocument * doc, const char * validationFile, char ** error);
        static bool validateWithRelaxNG(XMLDocument * doc, const char * validationFile, char ** error);

    };
}

#endif
