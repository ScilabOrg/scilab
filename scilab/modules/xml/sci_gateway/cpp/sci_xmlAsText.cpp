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
#include "XMLList.hxx"

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
int sci_xmlAsText(char * fname, int *_piKey)
{
    int id;
    SciErr err;
    int * addr = 0;
    XMLList * list = 0;
    const char ** pstStrings = 0;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(_piKey, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isXMLList(addr, _piKey) && !isXMLSet(addr, _piKey))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: XMLSet or XMLList expected.\n"), fname, 1);
        return 0;
    }

    id = getXMLObjectId(addr, _piKey);
    list = XMLObject::getFromId<XMLList>(id);
    if (!list)
    {
        Scierror(999, gettext("%s: XMLSet or XMLList does not exist.\n"), fname);
        return 0;
    }

    pstStrings = list->getContentFromList();

    err = createMatrixOfString(_piKey, Rhs + 1, 1, list->getSize(), const_cast<const char * const *>(pstStrings));
    delete[] pstStrings;
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999,_("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
