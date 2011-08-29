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

#ifndef __INSERTION_HPP__
#define __INSERTION_HPP__

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
#define __XML_CHECK_TYPE__(TYPEIN,REQUIREDTYPE,FIELD) if (typeid(TYPEIN) != typeid(REQUIREDTYPE)) \
    {                                                                   \
        Scierror(999, "%s: Wrong type to set %s field\n", fname, FIELD);  \
        return false;                                                   \
    }
/*--------------------------------------------------------------------------*/

/**
 * Sets the properties of a XMLDocument
 * A value will have the type T
 * @param fname function name
 * @param doc the XMLDocument
 * @param field te field name
 * @param value the new value
 */
template <class T>
bool setProperty(char * fname, XMLDocument & doc, const char * field, T & value)
{
    if (!strcmp("root", field))
    {
        if (typeid(T &) != typeid(XMLElement &) && typeid(T &) != typeid(std::string &))
        {
            Scierror(999, "%s: Wrong type to set %s field\n", fname, "root");
            return false;
        }
        if (typeid(T &) == typeid(XMLElement &))
        {
            doc.setRoot((XMLElement &)value);
        }
        else
        {
            char * error = 0;
            doc.setRoot((std::string &)value, &error);
            if (error)
            {
                Scierror(999, "%s: Not valid xml for root\n", fname);
                return false;
            }
        }
    }
    else if (!strcmp("url", field))
    {
        __XML_CHECK_TYPE__(T &, std::string &, "url");
        doc.setDocumentURL((std::string &)value);
    }
    else
    {
        Scierror(999, "%s: Unknown field: %s\n", fname, field);
        return false;
    }

    return true;
}
/*--------------------------------------------------------------------------*/

/**
 * Sets the properties of a XMLElement
 * A value will have the type T
 * @param fname function name
 * @param elem the XMLElement
 * @param field te field name
 * @param value the new value
 */
template <class T>
bool setProperty(char * fname, XMLElement & elem, const char * field, T & value)
{
    if (!strcmp("name", field))
    {
        __XML_CHECK_TYPE__(T &, std::string &, "name");
        elem.setNodeName((std::string &)value);
    }
    else if (!strcmp("namespace", field))
    {
        __XML_CHECK_TYPE__(T &, XMLNs &, "namespace");
        elem.setNodeNameSpace((XMLNs &)value);
    }
    else if (!strcmp("content", field))
    {
        __XML_CHECK_TYPE__(T &, std::string &, "content");
        elem.setNodeContent((std::string &)value);
    }
    else if (!strcmp("type", field))
    {
        Scierror(999, "%s: Field %s is not modifiable: %s\n", fname, "type");
        return false;
    }
    else if (!strcmp("parent", field))
    {
        Scierror(999, "%s: Field %s is not modifiable: %s\n", fname, "parent");
        return false;
    }
    else if (!strcmp("attributes", field))
    {
        __XML_CHECK_TYPE__(T &, XMLAttr &, "attributes");
        elem.setAttributes((XMLAttr &)value);
    }
    else if (!strcmp("children", field))
    {
        if (typeid(T &) != typeid(XMLElement &) && typeid(T &) != typeid(XMLNodeList &) && typeid(T &) != typeid(std::string &))
        {
            Scierror(999, "%s: Wrong type to set %s field\n", fname, "children");
            return false;
        }
        if (typeid(T &) == typeid(XMLElement &))
        {
            elem.setChildren((XMLElement &)value);
        }
        else if (typeid(T &) == typeid(XMLNodeList &))
        {
            elem.setChildren((XMLNodeList &)value);
        }
        else
        {
            elem.setChildren((std::string &)value);
        }
    }
    else
    {
        Scierror(999, "%s: Unknown field: %s\n", fname, field);
        return false;
    }

    return true;
}
/*--------------------------------------------------------------------------*/

/**
 * Function to handle insertion in different XMLObjects
 * @param fname the function name
 * @param fname_len the function name length
 */
template<class T, class U>
int sci_insertion(char * fname, unsigned long fname_len)
{
    T * a;
    U * b;
    int lhsid;
    SciErr err;
    int * fieldaddr = 0;
    int * rhsaddr = 0;
    int * lhsaddr = 0;
    char * field = 0;
    bool success;

    CheckLhs(1, 1);
    CheckRhs(3, 3);

    err = getVarAddressFromPosition(pvApiCtx, 1, &fieldaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, fieldaddr))
    {
        Scierror(999, "%s: Wrong type for input argument %i: String expected\n", fname, 1);
        return 0;
    }

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

    getAllocatedSingleString(pvApiCtx, fieldaddr, &field);
    lhsid = getXMLObjectId(lhsaddr);

    a = XMLObject::getFromId<T>(lhsid);
    if (!a)
    {
        Scierror(999, "%s: XML object does not exist\n", fname);
        return 0;
    }

    success = XMLRhsValue::get(fname, rhsaddr, &b);
    if (!success)
    {
        Scierror(999, "%s: Error in getting rhs argument\n", fname);
        return 0;
    }

    success = setProperty<U>(fname, *a, const_cast<char *>(field), *b);

    if (typeid(U) == typeid(std::string))
    {
        delete b;
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
