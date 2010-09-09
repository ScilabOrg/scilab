/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 
 * Copyright (C) DIGITEO - 2010 - Allan CORNET 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include "Scierror.h"
#include "stack-def.h" /* bsiz */
#include "error_internal.h"
#include "getNumberOfArgsAfterFormat.h"
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#define vsnprintf _vsnprintf
#endif
/*--------------------------------------------------------------------------*/ 
/* Scilab Error at C level */
/*--------------------------------------------------------------------------*/ 
int  Scierror(int iv,const char *fmt,...)
{
    int nbArgs = 0;
    int retval = 0;
    char s_buf[bsiz];
    va_list ap;

    if (fmt == NULL) return -1;

    va_start(ap, fmt);

    nbArgs = getNumberOfArgsAfterFormat(fmt, ap);

    if (nbArgs > 0)
    {
#if defined (vsnprintf) || defined (linux)
        retval = vsnprintf(s_buf, bsiz-1, fmt, ap );
#else
        retval = vsprintf(s_buf, fmt, ap );
#endif
        if (retval < 0) s_buf[bsiz-1] = '\0';

        retval = (int) strlen(s_buf);
        va_end(ap);
        error_internal(&iv, s_buf, ERROR_FROM_C);
    }
    else
    {
        retval = (int) strlen(fmt);
        error_internal(&iv, (char*)fmt, ERROR_FROM_C);
    }

    return retval;
}
/*--------------------------------------------------------------------------*/
