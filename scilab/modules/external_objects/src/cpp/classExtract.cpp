/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "ScilabGateway.hxx"
#include <cstring>

namespace org_modules_external_objects
{

int ScilabGateway::classExtract(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int tmpvar[2] = {0, 0};
    int * addr = 0;
    int idObj = 0;
    int eId = 0;
    int type = 0;
    int ret = 0;
    char * fieldName = 0;

    CheckInputArgument(pvApiCtx, 2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    eId = ScilabObjects::getEnvironmentId(addr, pvApiCtx);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(eId);
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    idObj = ScilabObjects::getArgumentId(addr, tmpvar, false, false, eId, pvApiCtx);

    try
    {
        fieldName = ScilabObjects::getSingleString(1, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        ScilabObjects::removeTemporaryVars(eId, tmpvar);
        throw;
    }

    if (helper.getNewAllowed() && !std::strcmp(fieldName, "new"))
    {
        options.setIsNew(true);
    }
    else
    {
        try
        {
            type = env.getfieldtype(idObj, fieldName);
        }
        catch (std::exception & e)
        {
            ScilabObjects::removeTemporaryVars(eId, tmpvar);
            freeAllocatedSingleString(fieldName);
            throw;
        }
    }

    ScilabObjects::removeTemporaryVars(eId, tmpvar);

    if (type == 0 || options.getIsNew())
    {
        // setMethodName will free fieldName
        options.setMethodName(fieldName);
        options.setObjId(idObj);
        OptionsHelper::setEnvId(eId);
        ScilabObjects::copyInvocationMacroToStack(Rhs + 1, env, pvApiCtx);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if (type == 1)
    {
        try
        {
            ret = env.getfield(idObj, fieldName);
        }
        catch (std::exception & e)
        {
            freeAllocatedSingleString(fieldName);
            throw;
        }

        freeAllocatedSingleString(fieldName);

        if (helper.getAutoUnwrap())
        {
            if (!ScilabObjects::unwrap(ret, Rhs + 1, eId, pvApiCtx))
            {
                try
                {
                    ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, eId, pvApiCtx);
                }
                catch (ScilabAbstractEnvironmentException & e)
                {
                    env.removeobject(ret);
                    throw;
                }
            }
            else
            {
                env.removeobject(ret);
            }
        }
        else
        {
            try
            {
                ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, eId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & e)
            {
                env.removeobject(ret);
                throw;
            }
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else
    {
        freeAllocatedSingleString(fieldName);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid name"));
    }

    return 0;
}
}
