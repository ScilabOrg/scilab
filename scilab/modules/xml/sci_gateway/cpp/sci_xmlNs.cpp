/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "XMLObject.hxx"
#include "XMLNs.hxx"
#include "XMLElement.hxx"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlNs(char *fname, unsigned long fname_len)
{
    int *addr = 0;
    SciErr err;
    XMLNs *ns = 0;
    XMLElement *elem = 0;
    char *prefix = 0;
    char *href = 0;

    CheckLhs(1, 1);
    CheckRhs(3, 3);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isXMLElem(addr, pvApiCtx))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A %s expected.\n"), fname, 1, "XMLElem");
        return 0;
    }

    elem = XMLObject::getFromId < XMLElement > (getXMLObjectId(addr, pvApiCtx));
    if (!elem)
    {
        Scierror(999, gettext("%s: XML element does not exist.\n"), fname);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isEmptyMatrix(pvApiCtx, addr))
    {
        prefix = 0;
    }
    else if (isStringType(pvApiCtx, addr) && checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        if (getAllocatedSingleString(pvApiCtx, addr, &prefix) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }
    else
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
    if (err.iErr)
    {
        if (prefix)
        {
            freeAllocatedSingleString(prefix);
        }
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        if (prefix)
        {
            freeAllocatedSingleString(prefix);
        }
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &href) != 0)
    {
        if (prefix)
        {
            freeAllocatedSingleString(prefix);
        }
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    ns = new XMLNs(*elem, prefix, href);

    if (prefix)
    {
        freeAllocatedSingleString(prefix);
    }

    if (href)
    {
        freeAllocatedSingleString(href);
    }

    if (!ns->createOnStack(Rhs + 1, pvApiCtx))
    {
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
