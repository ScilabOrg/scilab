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

#include "XMLDocument.hxx"

namespace org_modules_xml
{
    
    bool XMLValidation::validate(XMLDocument * doc, const char * method, const char * validationFile, char ** error)
    {
	if (!stricmp(method, "dtd"))
	{
	    return validateWithDTD(doc, 
    }
    
}
