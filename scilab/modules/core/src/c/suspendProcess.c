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

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include "aff_prompt.h"
#include "setCharDisplay.h"
#include "HistoryManager.h"
#include "init_tc_shell.h"
#include "suspendProcess.h"

/*
 * Function called by signal when SIGTSTP is caught.
 * Reset termcaps and colors before suspending Scilab.
 */
void suspendProcess(int signum)
{
    struct sigaction rien;

    struct sigaction ancien_traitement_a;

    rien.sa_handler = SIG_DFL;
    rien.sa_flags = 0;
    sigemptyset(&rien.sa_mask);
    sigaction(SIGTSTP, &rien, &ancien_traitement_a);
    setAttr(ATTR_RESET);
    setCharDisplay(DISP_RESET);
    printf("\nScilab is now suspended\n");
    kill(getpid(), SIGTSTP);
    sigaction(SIGTSTP, &ancien_traitement_a, NULL);
}

/*
 * Function called by signal when SIGCONT is caught
 * Set back Scilab colors and termcaps.
 */
void continueProcess(int signum)
{
    char *currentLine = NULL;

    currentLine = getSearchedTokenInScilabHistory();
    setCharDisplay(DISP_LAST_SET);
    setAttr(RAW);
    getPrompt(WRT_PRT);
    if (currentLine != NULL)
    {
        printf("%s", currentLine);
        fflush(stdout);
    }
}
