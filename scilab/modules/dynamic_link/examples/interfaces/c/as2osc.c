/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "sciprint.h"

/*************************************************************
 * Example of using a character string sent by the interface.
 * All 'a' are replaced by 'o'.
 *************************************************************/

/* ************ This code is deprecated. See api_scilab ************* */

int as2osc(char *thechain)
{
    static int k, l;

#pragma message("Deprecated code. See help('api_scilab')")

    l = strlen(thechain);
    sciprint("character string length %d\n", l);
    for (k = 0 ; k < l; k++)
    {
        if ( thechain[k] == 'a' ) thechain[k] = 'o';
    }
    return 0;
}

