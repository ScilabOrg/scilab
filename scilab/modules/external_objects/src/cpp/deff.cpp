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

int ScilabGateway::deff(char * fname, const int envId, void * pvApiCtx)
{
    static int ONE = 1;
    static int TWO = 2;
    static int THREE = 3;

    SciErr err;
    char * className = 0;
    char * methName = 0;
    char * funName = 0;
    int ret = 0;
    std::stringstream ss;
    char * str;

    CheckInputArgument(pvApiCtx, 3, 3);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper & helper = env.getOptionsHelper();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    className = ScilabObjects::getSingleString(1, pvApiCtx);

    try
    {
        ret = env.loadclass(className, helper.getAllowReload());
    }
    catch (std::exception & e)
    {
        freeAllocatedSingleString(className);
        throw;
    }

    try
    {
        methName = ScilabObjects::getSingleString(2, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        freeAllocatedSingleString(className);
        throw;
    }

    freeAllocatedSingleString(className);

    try
    {
        funName = ScilabObjects::getSingleString(3, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        freeAllocatedSingleString(methName);
        env.removeobject(ret);
        throw;
    }

    err = createMatrixOfString(pvApiCtx, ONE, 1, 1, (const char * const *)&funName);
    if (err.iErr)
    {
        freeAllocatedSingleString(funName);
        freeAllocatedSingleString(methName);
        env.removeobject(ret);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    ss << "y=" << funName << "(varargin)";
    freeAllocatedSingleString(funName);

    str = strdup(ss.str().c_str());
    err = createMatrixOfString(pvApiCtx, TWO, 1, 1, (const char * const *)&str);
    free(str);
    if (err.iErr)
    {
        env.removeobject(ret);
        freeAllocatedSingleString(methName);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    ss.str("");
    ss << "y=invoke_lu(int32(" << ret << "),int32(" << envId << "),\"" << methName << "\",varargin)";
    str = strdup(ss.str().c_str());

    freeAllocatedSingleString(methName);

    err = createMatrixOfString(pvApiCtx, THREE, 1, 1, (const char * const *)&str);
    free(str);
    if (err.iErr)
    {
        env.removeobject(ret);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    SciString(&ONE, "!_deff_wrapper", &ONE, &THREE);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
}
