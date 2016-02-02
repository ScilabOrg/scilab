/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "tohome.h"
#include "configvariable_interface.h"
#include "PromptToHome.h"
#ifndef _MSC_VER
#include "others/clrscr_nw.h"
#endif
/*--------------------------------------------------------------------------*/
BOOL tohome(void)
{
    BOOL bOK = FALSE;

    if ( getScilabMode() != SCILAB_STD )
    {
#ifdef _MSC_VER
        /* not on Windows */
        bOK = FALSE;
#else
        clrscr_nw();
        bOK = TRUE;
#endif
    }
    else
    {
        /* Java Console */
        bOK = PromptToHome();
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
