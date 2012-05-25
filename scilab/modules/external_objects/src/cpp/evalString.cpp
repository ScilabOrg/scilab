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

int ScilabGateway::evalString(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int row;
    int col;
    char ** code = 0;

    CheckInputArgumentAtLeast(pvApiCtx, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    for (int i = 1; i <= Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!isStringType(pvApiCtx, addr))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A string expected."), 1);
        }

        err = getVarDimension(pvApiCtx, addr, &row, &col);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if ((row < 1 || col != 1) && (col < 1 || row != 1))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid dimension for argument #%d: A row or a column expected."), 1);
        }

        if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &code))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        try
        {
            env.evalString(const_cast<const char **>(code), row * col);
        }
        catch (std::exception & e)
        {
            freeAllocatedMatrixOfString(row, col, code);
            throw;
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
}
