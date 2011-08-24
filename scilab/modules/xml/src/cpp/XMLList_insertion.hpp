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

#ifndef __XMLLIST_INSERT_HPP__
#define __XMLLIST_INSERT_HPP__

#include "XMLRhsValue.hxx"

extern "C"
{
#include <stdio.h>
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/

/**
 * Function to handle insertion in XMLNodeList
 * @param fname the function name
 * @param fname_len the function name length
 */
template <class T>
int sci_XMLList_insertion(char * fname, unsigned long fname_len)
{
    XMLNodeList * a;
    T * b;
    int lhsid;
    bool success;
    double index;
    SciErr err;
    void * ptrToFree = 0;
    int * indexaddr = 0;
    int * rhsaddr = 0;
    int * lhsaddr = 0;

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

    success = XMLRhsValue::get(fname, rhsaddr, &b, &ptrToFree);
    if (!success)
    {
        Scierror(999, "%s: Error in getting rhs argument\n", fname);
        return 0;
    }

    a->setElementAtPosition(index, *b);

    if (typeid(T) == typeid(std::string))
    {
        freeAllocatedSingleString((char *)ptrToFree);
    }

    if (a->createOnStack(Rhs + 1))
    {
        LhsVar(1) = Rhs + 1;
    }
    else
    {
        LhsVar(1) = 0;
    }

    PutLhsVar();

    return 0;
}

#endif
