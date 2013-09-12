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

#include <string>
#include <vector>

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"
#include "SplitString.hxx"

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
int sci_xmlDump(char *fname, unsigned long fname_len)
{
    XMLObject *obj = 0;
    int id;
    int type;
    int b;
    SciErr err;
    int *addr = 0;

    std::vector < std::string > lines;
    std::vector < const char *>clines;
    bool indent = true;

    CheckLhs(1, 1);
    CheckRhs(1, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    type = isXMLObject(addr, pvApiCtx);
    if (!type)
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A %s expected.\n"), fname, 1, "XML object");
        return 0;
    }

    id = getXMLObjectId(addr, pvApiCtx);
    obj = XMLObject::getFromId < XMLObject > (id);
    if (!obj)
    {
        Scierror(999, gettext("%s: XML object does not exist.\n"), fname);
        return 0;
    }

    if (Rhs == 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!isBooleanType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A %s expected.\n"), fname, 2, "boolean");
            return 0;
        }

        if (getScalarBoolean(pvApiCtx, addr, &b))
        {
            return 0;
        }
        indent = b != 0;
    }

    lines = std::vector < std::string > ();
    SplitString::split(obj->dump(indent), lines);
    clines = std::vector < const char *>(lines.size());

    for (unsigned int i = 0; i < lines.size(); i++)
    {
        clines[i] = lines[i].c_str();
    }

    if (clines.size())
    {
        err = createMatrixOfString(pvApiCtx, Rhs + 1, (int)lines.size(), 1, const_cast < const char *const *>(&(clines[0])));
    }
    else
    {
        err = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, 0);
    }

    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
