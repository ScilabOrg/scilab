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

#include "ScilabPythonEnvironment.hxx"
#include "ScilabGateway.hxx"

extern "C" {
#include "Scierror.h"
#include "gw_external_objects.h"
}

using namespace org_modules_external_objects_python;
using namespace org_modules_external_objects;

int sci_pyNpWrap(char * fname, unsigned long fname_len)
{
    SciErr sciErr;
    int * addr = 0;
    int val = 1;
    int idObj;
    int tmpvar[] = {0, 0};

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    const int envId = ScilabPythonEnvironment::start();
    ScilabPythonEnvironment & env = ScilabPythonEnvironment::getInstance();
    PythonOptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    if (Rhs == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (sciErr.iErr)
        {
            Scierror(999, "%s: An error occured: %s", fname, ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data")).what());
            return 0;
        }

        if (!isBooleanType(pvApiCtx, addr))
        {
            Scierror(999, "%s: An error occured: %s", fname, ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A boolean expected."), 1).what());
            return 0;
        }

        if (!isScalar(pvApiCtx, addr))
        {
            Scierror(999, "%s: An error occured: %s", fname, ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A single boolean expected."), 1).what());
            return 0;
        }

        getScalarBoolean(pvApiCtx, addr, &val);
    }

    bool old = helper.getUseNumpy();
    if (!old)
    {
        helper.setUseNumpy(true);
    }
    helper.setWrapSingleWithNumpy(true);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (sciErr.iErr)
    {
        helper.setWrapSingleWithNumpy(false);
        helper.setUseNumpy(old);
        Scierror(999, "%s: An error occured: %s", fname, ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data")).what());
        return 0;
    }

    try
    {
        idObj = ScilabObjects::getArgumentId(addr, tmpvar, val != 1, false, envId, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        helper.setWrapSingleWithNumpy(false);
        helper.setUseNumpy(old);
        Scierror(999, "%s: An error occured: %s", fname, e.what());
        return 0;
    }

    try
    {
        ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, idObj, envId, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        helper.setWrapSingleWithNumpy(false);
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        Scierror(999, "%s: An error occured: %s", fname, e.what());
        return 0;
    }

    if (!old)
    {
        helper.setUseNumpy(false);
    }
    helper.setWrapSingleWithNumpy(false);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
