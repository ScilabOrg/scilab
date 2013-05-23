/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "fileio_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "filemanager.hxx"
#include "function.hxx"

extern "C"
{
#include <stdio.h>
#include "expandPathVariable.h"
#include "warningmode.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_meof(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRet  = 0;
    int iFile = -1; //default file : last opened file

    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "meof", 0, 1);
        return types::Function::Error;
    }
    if (in.size() == 1)
    {
        if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "meof", 1);
            return types::Function::Error;
        }

        iFile = (int)in[0]->getAs<types::Double>()->get(0);
        switch (iFile)
        {
            case 0: // stderr
            case 5: // stdin
            case 6: // stdout
                Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "meof", iFile);
                return types::Function::Error;
        }
    }

    File* pF = FileManager::getFile(iFile);
    if (pF != NULL)
    {
        iRet = feof(pF->getFiledesc());
    }
    else
    {
        if (getWarningMode())
        {
            sciprint(_("%ls: Cannot check the end of file whose descriptor is %d: File is not active.\n"), "meof", iFile);
        }
        return types::Function::OK;
    }

    Double* pOut = new Double(iRet);
    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
