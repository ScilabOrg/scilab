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
#include "funcmanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "filemanager.hxx"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "MALLOC.h"
#include "removedir.h"
#include "isdir.h"
#include "expandPathVariable.h"
#include "sci_warning.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
}
/*--------------------------------------------------------------------------*/


Function::ReturnValue sci_meof(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRet  = 0;
    int iFile = -1; //default file : last opened file
    
    if(in.size() > 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"meof", 0, 1);
        return types::Function::Error;
    }
    if(in.size() == 1)
    {
        if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[1]->getAs<types::Double>()->isComplex())
        {
            ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A real expected.\n"), L"meof", 1);
            return types::Function::Error;
        }

        iFile = (int)in[0]->getAs<types::Double>()->get(0);
    }
    
    File* pF = FileManager::getFile(iFile);
    if(pF != NULL)
    {
        iRet = feof(pF->getFiledesc());
    }
    else
    {
        if (getWarningMode()) 
        {
            sciprintW(_W("%ls: Cannot check the end of file whose descriptor is %d: File is not active.\n"), L"meof", iFile);
        }
        return types::Function::OK;
    }
    
    Bool* pOut = new Bool(iRet);
    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
