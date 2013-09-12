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

int ScilabGateway::getClassName(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * tmpvar = 0;
    int idObj = 0;
    int * addr = 0;
    std::string * classNames = 0;
    const char ** str = 0;

    CheckInputArgumentAtLeast(pvApiCtx, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    classNames = new std::string[Rhs];
    tmpvar = new int[Rhs + 1];
    *tmpvar = 0;

    for (int i = 1; i <= Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;
            delete[] classNames;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        try
        {
            idObj = ScilabObjects::getArgumentId(addr, tmpvar, false, false, envId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & e)
        {
            delete[] tmpvar;
            delete[] classNames;
            throw;
        }

        try
        {
            classNames[i - 1] = env.getclassname(idObj);
        }
        catch (std::exception & e)
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;
            delete[] classNames;
            throw;
        }
    }

    ScilabObjects::removeTemporaryVars(envId, tmpvar);

    str = new const char*[Rhs];
    for (int i = 0; i < Rhs; i++)
    {
        str[i] = classNames[i].c_str();
    }

    err = createMatrixOfString(pvApiCtx, Rhs + 1, 1, Rhs, (const char * const *)str);
    delete[] classNames;
    delete[] str;

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
