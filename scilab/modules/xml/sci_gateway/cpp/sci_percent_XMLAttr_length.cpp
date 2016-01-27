/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

#include "XMLObject.hxx"
#include "XMLAttr.hxx"

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
int sci_percent_XMLAttr_length(char *fname, void* pvApiCtx)
{
    int id;
    SciErr err;
    double d;
    int *addr = 0;
    XMLAttr *attrs;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isXMLAttr(addr, pvApiCtx))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A %s expected.\n"), fname, 1, "XMLAttr");
        return 0;

    }

    id = getXMLObjectId(addr, pvApiCtx);
    attrs = XMLObject::getFromId < XMLAttr > (id);
    if (!attrs)
    {
        Scierror(999, gettext("%s: XML attribute does not exist.\n"), fname);
        return 0;
    }

    d = (double)attrs->getSize();
    createScalarDouble(pvApiCtx, Rhs + 1, d);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
