/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "function.hxx"
#include "string.hxx"
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
    /* DIR_SEPARATORW */
#include "machine.h"
#include "Scierror.h"
#include "localization.h"
#include "os_wcsicmp.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_filesep(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "filesep" , 0);
        return Function::Error;
    }

    if (_iRetCount != 1 && _iRetCount != -1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "filesep", 1);
        return Function::Error;
    }

    String* pS = new String(DIR_SEPARATORW);
    out.push_back(pS);
    return Function::OK;
    //static int n1,m1;
    //char *separator = NULL;

    //CheckRhs(0,0);
    //CheckLhs(1,1);

    //separator = os_strdup(DIR_SEPARATOR);

    //n1 = 1;
    //CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(separator), &m1),&n1,&separator);
    //LhsVar(1)=Rhs+1;
    //C2F(putlhsvar)();

    //if (separator) {FREE(separator);separator=NULL;}

    //return 0;
}
/*--------------------------------------------------------------------------*/

