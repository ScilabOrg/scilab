/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <cstring>
#include <cstdio>

#include "FieldsManager.hxx"
#include "EOFieldsGetter.hxx"
#include "ScilabObjects.hxx"

extern "C"
{
#include "api_scilab.h"
#include "sci_malloc.h"
}

using namespace org_modules_external_objects;

namespace org_modules_completion
{

const char ** EOFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    int envId = ScilabObjects::getEnvironmentId(mlist, NULL);
    int idObj = ScilabObjects::getExternalId(mlist, NULL);
    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabObjects::initialization(env, NULL);
    std::vector<std::string> fields;
    const char ** ret = 0;

    try
    {
        fields = env.getCompletion(idObj, fieldPath, fieldPathLen);
    }
    catch (const std::exception & /*e*/)
    {
        return 0;
    }

    *fieldsSize = (int)fields.size();
    ret = (const char **)MALLOC(sizeof(char *) **fieldsSize);

    for (int i = 0; i < *fieldsSize; i++)
    {
        ret[i] = os_strdup(fields.at(i).c_str());
    }

    return ret;
}
}
