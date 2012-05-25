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

int ScilabGateway::invoke_lu(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int typ = 0;
    int * addr = 0;
    int * listaddr = 0;
    int len = 0;
    int * tmpvar = 0;
    int idObj = 0;
    int * args = 0;
    int * child = 0;
    char * methName = 0;
    int * eId;
    int row, col;
    int ret = 0;

    CheckInputArgument(pvApiCtx, 4, 4);

    err = getVarAddressFromPosition(pvApiCtx, 4, &listaddr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    err = getVarType(pvApiCtx, listaddr, &typ);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (typ != sci_list)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A List expected."), 4);
    }

    err = getListItemNumber(pvApiCtx, listaddr, &len);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    // Get the environment id
    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    err = getVarType(pvApiCtx, addr, &typ);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (typ != sci_ints)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An Integer32 expected."), 2);
    }
    else
    {
        int prec;

        err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (prec != SCI_INT32)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An Integer32 expected."), 2);
        }

        err = getMatrixOfInteger32(pvApiCtx, addr, &row, &col, &eId);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(*eId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    // Get the object id
    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    err = getVarType(pvApiCtx, addr, &typ);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (typ != sci_ints)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An Integer32 expected."), 1);
    }
    else
    {
        int prec;
        int * id;

        err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (prec != SCI_INT32)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An Integer32 expected."), 1);
        }

        err = getMatrixOfInteger32(pvApiCtx, addr, &row, &col, &id);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        idObj = *id;
    }

    if (idObj == 0)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot invoke on null object."), __FILE__, __LINE__);
    }

    tmpvar = new int[len + 1];
    *tmpvar = 0;

    args = new int[len];

    for (int i = 0; i < len; i++)
    {
        err = getListItemAddress(pvApiCtx, listaddr, i + 1, &child);
        if (err.iErr)
        {
            delete[] args;
            ScilabObjects::removeTemporaryVars(*eId, tmpvar);
            delete[] tmpvar;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
        args[i] = ScilabObjects::getArgumentId(child, tmpvar, false, false, *eId, pvApiCtx);
        // If args[i] == -1 then we have a scilab variable which cannot be converted in a Java object.
        if (args[i] == - 1)
        {
            delete[] args;
            ScilabObjects::removeTemporaryVars(*eId, tmpvar);
            delete[] tmpvar;

            return 0;
        }
    }

    try
    {
        methName = ScilabObjects::getSingleString(3, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        delete[] args;
        ScilabObjects::removeTemporaryVars(*eId, tmpvar);
        delete[] tmpvar;
        throw;
    }

    try
    {
        ret = env.invoke(idObj, methName, args, len);
    }
    catch (std::exception & e)
    {
        delete[] args;
        ScilabObjects::removeTemporaryVars(*eId, tmpvar);
        delete[] tmpvar;
        freeAllocatedSingleString(methName);
        throw;
    }

    delete[] args;
    ScilabObjects::removeTemporaryVars(*eId, tmpvar);
    delete[] tmpvar;
    freeAllocatedSingleString(methName);

    if (ret == VOID_OBJECT)
    {
        PutLhsVar();

        return 0;
    }

    if (!ScilabObjects::unwrap(ret, Rhs + 1, *eId, pvApiCtx))
    {
        try
        {
            ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, *eId, pvApiCtx);
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

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
