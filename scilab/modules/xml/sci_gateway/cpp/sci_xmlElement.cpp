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

extern "C"
{
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "libxml/tree.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlElement(char * fname, int* pvApiCtx)
{
    org_modules_xml::XMLDocument * doc = 0;
    XMLElement * elem = 0;
    SciErr err;
    int * addr = 0;
    char * name = 0;

    CheckLhs(1, 1);
    CheckRhs(2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isXMLDoc(addr, pvApiCtx))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: A %s expected.\n"), fname, 1, "XMLDoc");
        return 0;
    }

    doc = XMLObject::getFromId<org_modules_xml::XMLDocument>(getXMLObjectId(addr, pvApiCtx));
    if (!doc)
    {
        Scierror(999, gettext("%s: XML Document does not exist.\n"), fname);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: A string expected.\n"), fname, 2);
        return 0;
    }

    getAllocatedSingleString(pvApiCtx, addr, &name);

    if (!strlen(name) || xmlValidateName((const xmlChar *)name, 0))
    {
        freeAllocatedSingleString(name);
        Scierror(999, gettext("%s: Bad input argument #%i: A valid XML name expected.\n"), fname, 2);
        return 0;
    }

    elem = new XMLElement(*doc, name);
    freeAllocatedSingleString(name);
    if (!elem->createOnStack(Rhs + 1, pvApiCtx))
    {
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
