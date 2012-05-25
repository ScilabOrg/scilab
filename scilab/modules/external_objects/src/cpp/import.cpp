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

int ScilabGateway::import(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int rows, cols;
    char ** className = 0;
    char ** name = 0;
    int named = 1;
    int * addr = 0;
    int ret = 0;
    int nbArgs = Rhs;

    CheckInputArgumentAtLeast(pvApiCtx, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    err = getVarAddressFromPosition(pvApiCtx, Rhs, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (isBooleanType(pvApiCtx, addr))
    {
        nbArgs = Rhs - 1;
        if (getScalarBoolean(pvApiCtx, addr, &named))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }

    if (nbArgs == 1)
    {
        err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!isStringType(pvApiCtx, addr))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A String expected."), 1);
        }

        if (getAllocatedMatrixOfString(pvApiCtx, addr, &rows, &cols, &className))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }
    else
    {
        className = (char**)MALLOC(sizeof(char *) * nbArgs);
        for (int i = 1; i <= nbArgs; i++)
        {
            err = getVarAddressFromPosition(pvApiCtx, i, &addr);
            if (err.iErr)
            {
                freeAllocatedMatrixOfString(1, i, className);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            if (!isStringType(pvApiCtx, addr))
            {
                freeAllocatedMatrixOfString(1, i, className);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A String expected."), i);
            }

            if (getAllocatedSingleString(pvApiCtx, addr, &(className[i - 1])))
            {
                freeAllocatedMatrixOfString(1, i, className);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }
        }

        rows = 1;
        cols = nbArgs;
    }

    name = new char*[rows * cols];

    for (int i = 0; i < rows * cols; i++)
    {
        name[i] = strrchr(className[i], '.');

        if (!name[i])
        {
            name[i] = className[i];
        }
        else if (name[i][1] == '\0')
        {
            freeAllocatedMatrixOfString(rows, cols, className);
            delete[] name;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("The class name cannot end with a dot."));
        }
        else
        {
            name[i]++;
        }

        if (named && isNamedVarExist(pvApiCtx, name[i]))
        {
            addr = 0;
            err = getVarAddressFromName(pvApiCtx, name[i], &addr);
            if (err.iErr || addr == 0 || !ScilabObjects::isExternalClass(addr, pvApiCtx))
            {
                freeAllocatedMatrixOfString(rows, cols, className);
                delete[] name;
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("A variable with this name is already existing"));
            }
        }
    }

    if (!named && rows * cols != Lhs)
    {
        freeAllocatedMatrixOfString(rows, cols, className);
        delete[] name;
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Bad number of output arguments"), __FILE__, __LINE__);
    }

    for (int i = 0; i < rows * cols; i++)
    {
        try
        {
            ret = env.loadclass(className[i], helper.getAllowReload());
        }
        catch (std::exception & e)
        {
            freeAllocatedMatrixOfString(rows, cols, className);
            delete[] name;
            throw;
        }

        if (named)
        {
            try
            {
                ScilabObjects::createNamedEnvironmentObject(EXTERNAL_CLASS, name[i], ret, envId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & e)
            {
                freeAllocatedMatrixOfString(rows, cols, className);
                delete[] name;
                throw;
            }
        }
        else
        {
            try
            {
                ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_CLASS, Rhs + i + 1, ret, envId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & e)
            {
                freeAllocatedMatrixOfString(rows, cols, className);
                delete[] name;
                env.removeobject(ret);
                throw;
            }
            LhsVar(i + 1) = Rhs + i + 1;
        }
    }

    freeAllocatedMatrixOfString(rows, cols, className);
    delete[] name;

    if (named)
    {
        LhsVar(1) = 0;
    }

    PutLhsVar();

    return 0;
}
}
