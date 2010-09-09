/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "machine.h"
#include "getNumberOfArgsAfterFormat.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
int getNumberOfArgsAfterFormat(const char *fmt, va_list args)
{
    int i = 0;

    if (fmt == NULL) return -1;

    if (args)
    {
        char *pStr = NULL;
        va_list myArgs;
        va_copy(myArgs, args);

        pStr = va_arg(myArgs, char *);
        while (pStr)
        {
            pStr = va_arg(myArgs, char *);
            i++;
        };
        va_end(myArgs);
    }

    return i;
}
/*--------------------------------------------------------------------------*/ 