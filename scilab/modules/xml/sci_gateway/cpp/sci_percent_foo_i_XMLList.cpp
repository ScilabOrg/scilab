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
#include "XMLElement.hxx"
#include "XMLNodeList.hxx"

extern "C"
{
#include <stdio.h>
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "xml_mlist.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_percent_foo_i_XMLList(char * fname, unsigned long fname_len)
{
    XMLNodeList * a;
    int lhsid;
    double index;
    SciErr err;
    char * prefix = 0;
    char * underscore = 0;
    char * format = 0;
    int * indexaddr = 0;
    int * rhsaddr = 0;
    int * lhsaddr = 0;
    int * retaddr = 0;
    char * retstr = 0;
    int iBegin = 2;
    int mrhs = 1;
    int mlhs = 1;

    CheckLhs(1, 1);
    CheckRhs(3, 3);

    err = getVarAddressFromPosition(pvApiCtx, 1, &indexaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isDoubleType(pvApiCtx, indexaddr))
    {
        Scierror(999, "%s: Wrong type for input argument %i: double expected\n", fname, 1);
        return 0;
    }

    getScalarDouble(pvApiCtx, indexaddr, &index);
    
    err = getVarAddressFromPosition(pvApiCtx, 2, &rhsaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 3, &lhsaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    lhsid = getXMLObjectId(lhsaddr);
    a = XMLObject::getFromId<XMLNodeList>(lhsid);
    if (!a)
    {
        Scierror(999, "%s: XML object do not exist\n", fname);
        return 0;
    }

    prefix = strdup(fname);
    underscore = strchr(prefix, '_');
    *underscore = '\0';
    format = (char *)MALLOC(sizeof(char *) * (strlen(prefix) + strlen("_xmlFormat") + 1));
    sprintf(format, "%s_xmlFormat", prefix);
    free(prefix);

    if (isNamedVarExist(pvApiCtx, format))
    {
        SciString(&iBegin, format, &mlhs, &mrhs);
        FREE(format);
        err = getVarAddressFromPosition(pvApiCtx, iBegin, &retaddr);
        if (err.iErr)
        {
            printError(&err, 0);
            return 0;
        }

        if (!isStringType(pvApiCtx, retaddr))
        {
            Scierror(999, "%s: xmlFormat must return a string\n", fname);
            return 0;
        }

        getAllocatedSingleString(pvApiCtx, retaddr, &retstr);
        if (retstr)
        {
            a->setElementAtPosition(index, std::string(retstr));
            a->createOnStack(Rhs + 1);
            LhsVar(1) = Rhs + 1;
        }
    }
    else
    {// todo mettre %s_xml...
        Scierror(999, "%s: You must define %s_xmlFormat function\n", fname, format);
        FREE(format);
        return 0;
    }

    PutLhsVar();

    return 0;
}
