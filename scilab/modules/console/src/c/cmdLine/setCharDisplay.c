/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include "setCharDisplay.h"

/* Set color and other settings about charcter display */
void setCharDisplay(const char *setting)
{
    static char *currentSetting = NULL;

    static char *lastSetting = NULL;

    if (setting == NULL)
    {
        if (lastSetting != NULL)
        {
            setting = lastSetting;
        }
        else
        {
            setting = currentSetting;
        }
    }
    printf("\033[%sm", setting);
    lastSetting = currentSetting;
    currentSetting = setting;
    fflush(stdout);
}
