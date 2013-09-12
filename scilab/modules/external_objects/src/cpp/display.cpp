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

int ScilabGateway::display(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int row = 0, col = 0;
    int * id = 0;
    int eId;

    CheckInputArgument(pvApiCtx, 1, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (!ScilabObjects::isExternalObj(addr, pvApiCtx) && !ScilabObjects::isExternalClass(addr, pvApiCtx))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: External Object expected."), 1);
    }

    eId = ScilabObjects::getEnvironmentId(addr, pvApiCtx);
    if (eId != envId)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Incompatible External Object"));
    }

    err = getMatrixOfInteger32InList(pvApiCtx, addr, EXTERNAL_OBJ_ID_POSITION, &row, &col, &id);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    ScilabStream ss;
    ss << env.getrepresentation(*id) << std::endl;

    PutLhsVar();

    return 0;
}
}
