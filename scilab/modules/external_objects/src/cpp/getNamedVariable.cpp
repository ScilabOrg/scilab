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

namespace org_modules_external_objects
{

int ScilabGateway::getNamedVariable(char * fname, const int envId, void * pvApiCtx)
{
    char * varName = 0;
    int idObj;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    varName = ScilabObjects::getSingleString(1, pvApiCtx);

    try
    {
        idObj = env.getNamedVariable(varName);
    }
    catch (std::exception & e)
    {
        freeAllocatedSingleString(varName);
        throw;
    }
    freeAllocatedSingleString(varName);

    try
    {
        ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, idObj, envId, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        env.removeobject(idObj);
        throw;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
