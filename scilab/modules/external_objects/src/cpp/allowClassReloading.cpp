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

int ScilabGateway::allowClassReloading(char * fname, const int envId, void * pvApiCtx)
{
    SciErr sciErr;
    int * addr = 0;
    int val = 0;
    int iType = 0;

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);

    options.setIsNew(false);

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (Rhs == 0)
    {
        createScalarBoolean(pvApiCtx, 1, (int)helper.getAllowReload());
        LhsVar(1) = 1;
        PutLhsVar();
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (sciErr.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    sciErr = getVarType(pvApiCtx, addr, &iType);
    if (sciErr.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (iType != sci_boolean)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A boolean expected."), 1);
    }

    if (!isScalar(pvApiCtx, addr))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A single boolean expected."), 1);
    }

    getScalarBoolean(pvApiCtx, addr, &val);
    helper.setAllowReload(val == 1);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
}
