/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "xml.h"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

#include "XMLObject.hxx"

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlSetAttributes(char *fname, unsigned long fname_len)
{
    int id;
    SciErr err;
    int *addr = 0;
    XMLObject *obj = 0;
    char **keyValue = 0;
    int rows;
    int cols;

    CheckLhs(1, 1);
    CheckRhs(2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isXMLAttr(addr, pvApiCtx) && !isXMLElem(addr, pvApiCtx) && !isXMLList(addr, pvApiCtx) && !isXMLSet(addr, pvApiCtx))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A XMLAttr or a XMLElem or a XMLList or a XMLSet expected.\n"), fname, 1);
        return 0;
    }

    id = getXMLObjectId(addr, pvApiCtx);
    obj = XMLObject::getFromId < XMLObject > (id);
    if (!obj)
    {
        Scierror(999, gettext("%s: XML attribute does not exist.\n"), fname);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, addr, &rows, &cols, &keyValue))
    {
        return 0;
    }

    if (rows == 0 || (cols != 2 && cols != 3))
    {
        freeAllocatedMatrixOfString(rows, cols, keyValue);
        Scierror(999, gettext("%s: Wrong size for input argument #%d: Matrix nx2 or nx3 of strings expected.\n"), fname, 2);
        return 0;
    }

    if (cols == 2)
    {
        obj->setAttributeValue(const_cast < const char **>(keyValue), const_cast < const char **>(keyValue + rows), rows);
    }
    else
    {
        obj->setAttributeValue(const_cast < const char **>(keyValue), const_cast < const char **>(keyValue + rows),
                               const_cast < const char **>(keyValue + 2 * rows), rows);
    }
    freeAllocatedMatrixOfString(rows, cols, keyValue);

    obj->createOnStack(Rhs + 1, pvApiCtx);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
