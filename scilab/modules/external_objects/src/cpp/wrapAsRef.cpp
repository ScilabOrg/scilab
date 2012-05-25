/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ScilabGateway.hxx"

namespace org_modules_external_objects
{

int ScilabGateway::wrapAsRef(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * tmpvar = 0;
    int * addr = 0;
    char * varName = 0;

    if (Rhs == 0)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong number of arguments : more than 1 argument expected."));
    }

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    CheckOutputArgument(pvApiCtx, Rhs, Rhs);

    tmpvar = new int[Rhs + 1];
    *tmpvar = 0;

    for (int i = 1; i < Rhs + 1; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!isScalar(pvApiCtx, addr) || !isStringType(pvApiCtx, addr))
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Can only wrap as a reference a named variable"));
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &varName))
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        err = getVarAddressFromName(pvApiCtx, varName, &addr);
        freeAllocatedSingleString(varName);
        if (err.iErr)
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        int idObj = ScilabObjects::getArgumentId(addr, tmpvar, true, false, envId, pvApiCtx);
        if (idObj == -1)
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;

            if (i == 1)
            {
                OverLoad(1);
                return 0;
            }
            else
            {
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot wrap argument %d."), i);
            }
        }

        try
        {
            ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + i, idObj, envId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & e)
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;
            throw;
        }

        LhsVar(i) = Rhs + i;
    }

    PutLhsVar();

    return 0;
}
}
