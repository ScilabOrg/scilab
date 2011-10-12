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
#include "XMLRhsValue.hxx"

extern "C"
{
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlReadStr(char * fname, int* pvApiCtx)
{
    org_modules_xml::XMLDocument * doc;
    SciErr err;
    int * addr = 0;
    std::string * code;
    std::string error;
    bool validate = false;
    int validateParam;

    CheckLhs(1, 1);
    CheckRhs(1, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: A string expected.\n"), fname, 1);
        return 0;
    }

    if (!XMLRhsValue::get(fname, addr, &code, pvApiCtx))
    {
        return 0;
    }

    if (Rhs == 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            delete code;
            printError(&err, 0);
            return 0;
        }

        if (!isBooleanType(pvApiCtx, addr))
        {
            delete code;
            Scierror(999, gettext("%s: Wrong type for input argument #%i: A boolean expected.\n"), fname, 2);
            return 0;
        }

        getScalarBoolean(pvApiCtx, addr, &validateParam);
        validate = validateParam != 0;
    }

    doc = new org_modules_xml::XMLDocument(*code, validate, &error);
    delete code;

    if (!error.empty())
    {
        delete doc;
        Scierror(999, gettext("%s: Cannot parse the string:\n%s"), fname, error.c_str());
        return 0;
    }

    if (!doc->createOnStack(Rhs + 1, pvApiCtx))
    {
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
