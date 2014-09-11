/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "TerminateWindows_tools.h"
#include "sci_mode.h"
#include "console.h"
#include "InnosetupMutex.h"
/*--------------------------------------------------------------------------*/
BOOL TerminateWindows_tools(void)
{
    BOOL bOK = FALSE;
    int scilabMode = getScilabMode();

    switch (scilabMode)
    {
        case SCILAB_STD:
        {
            //Since Scilab 6 All scilab mode are launch in console mode
            //So we cannot close owner console
            //CloseScilabConsole();
            closeInnosetupMutex();
            bOK = TRUE;
        }
        break;

        case SCILAB_NW:
        {
            RestoreConsoleColors();
            RestoreExitButton();
            bOK = TRUE;
        }
        break;

        default:
            break;
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
