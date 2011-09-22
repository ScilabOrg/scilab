/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ScilabPreferences.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_preferences.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_preferences;

/*--------------------------------------------------------------------------*/
int sci__setAtomsTbxInfos(char * fname, unsigned long fname_len)
{
    SciErr err;
    int * addr = 0;
    char ** tbxName = 0;
    char ** tbxPath = 0;
    char ** tbxConfName = 0;
    char ** atomsName = 0;
    char ** atomsPath = 0;
    char ** atomsConfName = 0;
    char *** array[] = {&tbxName, &tbxPath, &tbxConfName, &atomsName, &atomsPath, &atomsConfName};
    int rows[6];
    int cols[6];
    bool error = false;

    CheckLhs(1, 1);
    CheckRhs(6, 6);

    for (int i = 0; i < Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 1, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            return 0;
        }

        err = getVarDimension(pvApiCtx, addr, rows + i, cols + i);
        if (err.iErr)
        {
            printError(&err, 0);
            return 0;
        }

        if (rows[i] && cols[i])
        {
            getAllocatedMatrixOfString(pvApiCtx, addr, rows + i, cols + i, array[i]);
        }
    }

    try
    {
        ScilabPreferences::setToolboxesInfos(getScilabJavaVM(), tbxName, rows[0] * cols[0], tbxPath, rows[1] * cols[1], tbxConfName, rows[2] * cols[2]);
        ScilabPreferences::setAtomsInfos(getScilabJavaVM(), atomsName, rows[3] * cols[3], atomsPath, rows[4] * cols[4], atomsConfName, rows[5] * cols[5]);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arised:\n%s"), fname, e.what());
        error = true;
    }

    for (int i = 0; i < Rhs; i++)
    {
        if (array[i])
        {
            freeAllocatedMatrixOfString(rows[i], cols[i], *(array[i]));
        }
    }

    if (!error)
    {
        LhsVar(1) = 0;
        PutLhsVar();
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
