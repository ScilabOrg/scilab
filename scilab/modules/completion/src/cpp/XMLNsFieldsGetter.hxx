/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __XMLNSFIELDSGETTER_HXX__
#define __XMLNSFIELDSGETTER_HXX__

#include <string>
#include <cstring>

#include "FieldsGetter.hxx"
#include "XMLFieldsGetter.hxx"
#include "xmlObjects.h"

extern "C"
{
#include "SCIMALLOC.h"
#include "os_strdup.h"
}

using namespace org_modules_xml;

namespace org_modules_completion
{

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to manage the retrievment of the fields name of a XML object
 */
class XMLNsFieldsGetter : public XMLFieldsGetter
{

public :
    XMLNsFieldsGetter() { }

    virtual ~XMLNsFieldsGetter() { }

    virtual const char ** getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const;

    static const char ** getFieldsName(const XMLNs * ns, char ** fieldPath, const int fieldPathLen, int * fieldsSize);
};
}

#endif
