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

#include <vector>

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"

extern "C"
{
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlDump(char *fname, unsigned long fname_len)
{
    int id;
    int type;
    SciErr err;
    int *addr = 0;
    char * dump;
    std::string str;
    std::vector<char *> vector;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    type = isXMLObject(addr);
    if (!type)
    {
        Scierror(999, "%s: Wrong type for input argument %i: %s expected\n", fname, 1, "XML object");
        return 0;
    }

    id = getXMLObjectId(addr);
    str = XMLObject::getFromId<XMLObject>(id)->dump();
    dump = const_cast<char *>(str.c_str());
    vector = std::vector<char *>();

    dump = strtok(dump, "\n\r");
    while (dump)
    {
        vector.push_back(dump);
        dump = strtok(0, "\n\r");
    }

    if (vector.size())
    {
        err = createMatrixOfString(pvApiCtx, Rhs + 1, vector.size(), 1, const_cast<const char * const *>(&(vector[0])));
    }
    else
    {
        err = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, 0);
    }

    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
