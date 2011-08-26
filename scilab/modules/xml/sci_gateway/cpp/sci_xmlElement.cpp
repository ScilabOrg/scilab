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
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlElement(char * fname, unsigned long fname_len)
{
    XMLDocument * doc = 0;
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

    if (!isXMLDoc(addr))
    {
        Scierror(999, "%s: Wrong type for input argument %i: XMLDocument expected\n", fname, 1);
        return 0;
    }

    doc = XMLObject::getFromId<XMLDocument>(getXMLObjectId(addr));
    if (!doc)
    {
        Scierror(999, "%s: XML Document do not exist\n", fname);
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
        Scierror(999, "%s: Wrong type for input argument %i: String expected\n", fname, 2);
        return 0;
    }

    getAllocatedSingleString(pvApiCtx, addr, &name);

    if (!name || !strlen(name) || xmlValidateName((const xmlChar *)name, 0))
    {
        freeAllocatedSingleString(name);
        Scierror(999, "%s: Bad input argument %i: valid name expected\n", fname, 2);
        return 0;
    }

    elem = new XMLElement(*doc, name);
    if (!elem->createOnStack(Rhs + 1))
    {
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
