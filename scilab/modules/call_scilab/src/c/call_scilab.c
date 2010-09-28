/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include <stdlib.h>
#include "BOOL.h"
#include "call_scilab.h"
#include "MALLOC.h"
#include "sci_mode.h"
#include "fromc.h"
#include "LaunchScilabSignal.h"
#include "localization.h"
#include "isdir.h"
#include "sci_path.h"
#include "scilabDefaults.h"
#include "sci_tmpdir.h"
#include "inisci-c.h"
#include "scirun.h"
#include "sciquit.h"
#include "storeCommand.h"
#include "WindowList.h"
#include "../../core/src/c/TerminateCore.h"
#include "os_strdup.h"
#include "call_scilab_engine_state.h"
#include "api_scilab.h"

#ifdef _MSC_VER
#include "SetScilabEnvironmentVariables.h"
#include "getScilabDirectory.h"
#endif
/*--------------------------------------------------------------------------*/
static CALL_SCILAB_ENGINE_STATE csEngineState = CALL_SCILAB_ENGINE_STOP;
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static void SetSciEnv(void)
{
    char *ScilabDirectory=NULL;

    ScilabDirectory = getScilabDirectory(TRUE);

    if (ScilabDirectory == NULL)
    {
        MessageBox (NULL, "getScilabDirectory()", "Error" , MB_ICONSTOP | MB_OK);
        exit(1);
    }
    SetScilabEnvironmentVariables(ScilabDirectory);

    if (ScilabDirectory){FREE(ScilabDirectory);ScilabDirectory=NULL;}

}
#endif
/*--------------------------------------------------------------------------*/
void DisableInteractiveMode(void)
{
    setScilabMode(SCILAB_NWNI);
}
/*--------------------------------------------------------------------------*/
BOOL StartScilab(char *SCIpath,char *ScilabStartup, int Stacksize) {
    return Call_ScilabOpen (SCIpath, TRUE, ScilabStartup, Stacksize) == 0;
}
/*--------------------------------------------------------------------------*/
/**
 * Start Scilab engine
 * Function created in the context of javasci v2.
 * This function is just like StartScilab but provides more error messages
 * in case or error. For now, it is only used in javasci v2 but it might
 * be public sooner or later.
 * @return
 * 0: success
 * -1: already running
 * -2: Could not find SCI
 * -3: No existing directory
 * Any other positive integer: A Scilab internal error
 */
int Call_ScilabOpen(char* SCIpath, BOOL advancedMode, char *ScilabStartup, int Stacksize)
{
    #define FORMAT_SCRIPT_STARTUP "exec(\"%s\",-1);quit;"
    char *ScilabStartupUsed = NULL;
    char *InitStringToScilab = NULL;
    int StacksizeUsed = 0;
    int lengthStringToScilab = 0;

    static int iflag = -1, ierr = 0;

    if (advancedMode == FALSE)
    {
        DisableInteractiveMode();
    }else{
        setScilabMode(SCILAB_API);
    }

    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STARTED) return -1;

    SetFromCToON();

    InitializeLaunchScilabSignal();

    if (SCIpath == NULL) /* No SCIpath provided... */
    {
#ifdef _MSC_VER
        SetSciEnv(); /* Windows has a way to detect it */
#else
        /* Other doesn't */
        fprintf(stderr,"StartScilab: Could not find SCI\n");
        return -2;
#endif
    }
    else
    {
        if (!isdir(SCIpath))
        {
            /* Check if the directory actually exists */
            fprintf(stderr,"StartScilab: Could not find the directory %s\n",SCIpath);
            return FALSE;
        }
        else
        {
            char env[2048];
            setSCI(SCIpath);
            sprintf(env,"SCI=%s",SCIpath);
            putenv(env);
        }
    }

    if (ScilabStartup == NULL)
    {
        ScilabStartupUsed = os_strdup(DEFAULTSCILABSTARTUP);
    }
    else
    {
        ScilabStartupUsed = os_strdup(ScilabStartup);
    }

    if (Stacksize==NULL || Stacksize == -1)
    {
        StacksizeUsed = DEFAULTSTACKSIZE;
    }
    else
    {
        StacksizeUsed = Stacksize;
    }

    /* creates TMPDIR */
    defineTMPDIR();

    /* Scilab Initialization */
    C2F(inisci)(&iflag,&StacksizeUsed,&ierr);
    if ( ierr > 0 ) return ierr;

    lengthStringToScilab = (int)(strlen(FORMAT_SCRIPT_STARTUP) + strlen(ScilabStartupUsed + 1));
    InitStringToScilab = (char*)MALLOC(lengthStringToScilab*sizeof(char));
    sprintf(InitStringToScilab,FORMAT_SCRIPT_STARTUP,ScilabStartupUsed);

    C2F(scirun)(InitStringToScilab,(long int)strlen(InitStringToScilab));

    if (ScilabStartupUsed) {FREE(ScilabStartupUsed);ScilabStartupUsed=NULL;}
    if (InitStringToScilab) {FREE(InitStringToScilab);InitStringToScilab=NULL;}

    setCallScilabEngineState(CALL_SCILAB_ENGINE_STARTED);
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL TerminateScilab(char *ScilabQuit)
{
    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STARTED)
    {
        if (getScilabMode() != SCILAB_NWNI)
        {
            ExitScilab();
        }
        else
        {
            TerminateCorePart2();
        }
        ReleaseLaunchScilabSignal();
        setCallScilabEngineState(CALL_SCILAB_ENGINE_STOP);
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
/**
* function called javasci
*/
void ScilabDoOneEvent(void)
{
    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STARTED)
    {
        if ( getScilabMode() != SCILAB_NWNI )
        {
            C2F(scirun)("quit;", (int)strlen("quit;"));
        }
    }
}
/*--------------------------------------------------------------------------*/
int ScilabHaveAGraph(void)
{
    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STARTED)
    {
        return sciHasFigures();
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
CALL_SCILAB_ENGINE_STATE setCallScilabEngineState(CALL_SCILAB_ENGINE_STATE state)
{
    csEngineState = state;
    return csEngineState;
}
/*--------------------------------------------------------------------------*/
CALL_SCILAB_ENGINE_STATE getCallScilabEngineState(void)
{
    return csEngineState;
}
/*--------------------------------------------------------------------------*/
sci_types getVariableType(char *varName) {
    int iSciType = -1;
    SciErr sciErr = getNamedVarType(NULL, (char*)varName, &iSciType);
    if (sciErr.iErr == API_ERROR_NAMED_UNDEFINED_VAR) {
        return -2;
    }
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return (sci_types) iSciType;
}
/*--------------------------------------------------------------------------*/
