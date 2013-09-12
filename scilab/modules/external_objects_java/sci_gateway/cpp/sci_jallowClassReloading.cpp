/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C" {
#include "Scierror.h"
#include "gw_external_objects_java.h"
}

#include "ScilabJavaEnvironment.hxx"
#include "ScilabGateway.hxx"

using namespace org_scilab_modules_external_objects_java;
using namespace org_modules_external_objects;

int sci_jallowClassReloading(char * fname, unsigned long fname_len)
{
    try
    {
        const int envId = ScilabJavaEnvironment::start();
        JavaOptionsSetter setter = ScilabJavaEnvironment::getInstance().getOptionsHelper().getSetter(JavaOptionsSetter::ALLOWRELOAD);
        return ScilabGateway::getsetOptions(fname, envId, setter, pvApiCtx);
    }
    catch (std::exception & e)
    {
        Scierror(999, "%s: An error occured: %s", fname, e.what());
        return 0;
    }

    return 0;
}
