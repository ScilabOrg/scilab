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
    struct sigaction signalDefaultSettings;

    struct sigaction lastSignalSettings;

    /* New structure is created to reset handler to default. */
    signalDefaultSettings.sa_handler = SIG_DFL;
    signalDefaultSettings.sa_flags = 0;
    sigemptyset(&signalDefaultSettings.sa_mask);
    /* Then apply settings... */
    sigaction(SIGTSTP, &signalDefaultSettings, &lastSignalSettings);
    /* ... And reset termcaps and Characters display. */
    setAttr(ATTR_RESET);
    setCharDisplay(DISP_RESET);
    /* Suspend Scilab. */
    if (kill(getpid(), SIGTSTP))
    {
        fprintf(stderr, "\nCannot suspend scilab\n");
    }
    sleep(1);
    /* Set back handler to reset settings if Scilab is suspended a new time. */
    sigaction(SIGTSTP, &lastSignalSettings, NULL);
}

/*
 * Function called by signal when SIGCONT is caught
 * Set back Scilab colors and termcaps.
 */
void continueProcess(int signum)
{
    char *currentLine = NULL;

    /* Set termcaps and colors to Scilab Console settings. */
    setCharDisplay(DISP_LAST_SET);
    setAttr(RAW);
    /* Display Prompt. */
    getPrompt(WRT_PRT);
    /* Get the current edited line and display it if there is any. */
    currentLine = getSearchedTokenInScilabHistory();
    if (currentLine != NULL)
    {
        printf("%s", currentLine);
        fflush(stdout);
    }
}
