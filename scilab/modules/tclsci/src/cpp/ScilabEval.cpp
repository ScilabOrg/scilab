/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab-Enterprises - Cedric Delamarre
  *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "configvariable.hxx"
#include "threadmanagement.hxx"

extern "C"
{
#include "TCL_Global.h"
#include "ScilabEval.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "storeCommand.h"
#include "os_string.h"
}

/*--------------------------------------------------------------------------*/
int TCL_EvalScilabCmd(ClientData clientData, Tcl_Interp * theinterp, int objc, CONST char ** argv)
{
    int ierr = 0;
    int isInterruptible = 1;
    char *pstCommand = NULL;

    if (argv[1] != (char *)0)
    {
        int ind = 0;
        pstCommand = os_strdup(argv[1]);
        if (pstCommand == NULL)
        {
            sciprint(_("%s: No more memory.\n"), "TCL_EvalScilabCmd");
            return TCL_ERROR;
        }

        std::string str(pstCommand);

        size_t pos = str.find('\n');
        while (pos != std::string::npos)
        {
            str.replace(pos, 1, "\\n");
            pos = str.find('\n');
        }

        pstCommand =  strdup(str.c_str());


        if (strncmp(pstCommand, "flush", 5) == 0)
        {
            // Remove me from threadId list
            ThreadManagement::LockRunner();
            types::ThreadId* pThreadMe = ConfigVariable::getLastThreadAndRemove();
            ThreadManagement::UnlockRunner();

            // Wake up Runner waiting for my execution ends
            ThreadManagement::SendAwakeRunnerSignal();

            // Wait for execution of all command stored
            if (isEmptyCommandQueue() == 0)
            {
                ThreadManagement::WaitForEmptyQueueSignal();
            }

            // Add me again in threadId list to terminate execution
            ConfigVariable::addThread(pThreadMe);
        }
        else if ( (argv[2] != (char *)0) && (strncmp(argv[2], "sync", 4) == 0) )
        {
            // Remove me from threadId list
            ThreadManagement::LockRunner();
            types::ThreadId* pThreadMe = ConfigVariable::getLastThreadAndRemove();
            ThreadManagement::UnlockRunner();

            isInterruptible = ( (argv[3] != (char *)0) && (strncmp(argv[3], "seq", 3) == 0) ) ? 0 : 1;
            StoreCommandWithFlags(pstCommand, 1/*is prioritary*/, isInterruptible, TCLSCI/*command origin*/);

            // wait for command execution ends
            ThreadManagement::WaitForTCLExecDoneSignal();

            // Add me again in threadId list to terminate execution
            ConfigVariable::addThread(pThreadMe);
        }
        else
        {
            isInterruptible = ( (argv[2] != (char *)0) && (strncmp(argv[2], "seq", 3) == 0) ) ? 0 : 1;
            StoreCommandWithFlags(pstCommand, 0/*is prioritary*/, isInterruptible, NONE/*command origin*/);
            if (isInterruptible)
            {
                Tcl_SetResult(theinterp, NULL, NULL);
            }
        }

        FREE(pstCommand);
    }
    else
    {
        /* ScilabEval called without argument */
        Scierror(999, _("%s: Wrong number of input argument(s): at least one expected.\n"), "TCL_EvalScilabCmd");
    }

    return TCL_OK;
}
