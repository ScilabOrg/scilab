/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005-2008 - INRIA - Allan CORNET
* Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#ifndef _MSC_VER
#include <dirent.h>
#include <ctype.h>
#else
#include "EnvTclTk.h"
#include "strdup_windows.h"
#endif
#include "InitTclTk.h"
#include "setgetSCIpath.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "scilabmode.h"
#include "ScilabEval.h"
#include "TCL_Command.h"
#include "GlobalTclInterp.h"
#include "BOOL.h"
#include "PATH_MAX.h"
#include "fromjava.h"
/*--------------------------------------------------------------------------*/
BOOL TK_Started=FALSE;

/* The tclLoop thread Id
in order to wait it ends when closing Scilab */
__threadId TclThread;

__threadSignal InterpReady;
__threadSignalLock InterpReadyLock;


/*--------------------------------------------------------------------------*/
static char *GetSciPath(void);
/*--------------------------------------------------------------------------*/
static void *DaemonOpenTCLsci(void* in)
/* Checks if tcl/tk has already been initialised and if not */
/* initialise it. It must find the tcl script */
{
	char *SciPath=NULL;
	char TkScriptpath[PATH_MAX];
	char MyCommand[2048];
	BOOL tkStarted=TRUE;

#ifndef _MSC_VER
	DIR *tmpdir=NULL;
#endif

	FILE *tmpfile2=NULL;

	SciPath=GetSciPath();

	/* test SCI validity */
	if (SciPath==NULL)
	{
		sciprint(_("The SCI environment variable is not set.\nTCL initialisation failed !\n"));
	}



#ifdef TCL_MAJOR_VERSION
#ifdef TCL_MINOR_VERSION
#if TCL_MAJOR_VERSION >= 8
#if TCL_MINOR_VERSION > 0
	Tcl_FindExecutable(" ");
#endif
#endif
#endif
#endif

#ifdef _MSC_VER

	strcpy(TkScriptpath, SciPath);
	strcat(TkScriptpath, "/modules/tclsci/tcl/TK_Scilab.tcl");

	tmpfile2 = fopen(TkScriptpath,"r");
	if (tmpfile2==NULL)
	{
		sciprint(_("Unable to find Tcl initialisation scripts.\nCheck your SCI environment variable.\nTcl initialisation failed !"));
	}
	else fclose(tmpfile2);
#else
	tmpdir=opendir(SciPath);
	if (tmpdir==NULL)
	{
		sciprint(_("The SCI environment variable is not set.\nTcl initialisation failed !\n"));
	}
	else closedir(tmpdir);
	strcpy(TkScriptpath,SciPath);
	strcat(TkScriptpath,"/modules/tclsci/tcl/TK_Scilab.tcl");
	tmpfile2 = fopen(TkScriptpath,"r");
	if (tmpfile2==NULL)
	{
		sciprint(_("Unable to find Tcl initialisation scripts.\nCheck your SCI environment variable.\nTcl initialisation failed !"));
	}
	else fclose(tmpfile2);
#endif /* _MSC_VER */

	if (getTclInterp() == NULL)
	{
		releaseTclInterp();
		initTclInterp();

#ifdef _MSC_VER
		/* Initialize TCL_LIBRARY & TK_LIBRARY variables environment */
		/* Windows only */
		SetTclTkEnvironment(SciPath);
#endif

		if ( getTclInterp() == NULL )
		{
			Scierror(999,_("Tcl Error: Unable to create Tcl interpreter (Tcl_CreateInterp).\n"));
		}
		releaseTclInterp();

		if ( Tcl_Init(getTclInterp()) == TCL_ERROR)
		{
			Scierror(999,_("Tcl Error: Error during the Tcl initialization (Tcl_Init).\n"));
			if (IsFromJava()){
				/* When we are calling this stuff from javasci in the binary,
				 * not finding TCL_LIBRARY is causing issue to display the 
				 * actual error (infinity loop)
				 * See: http://bugzilla.scilab.org/show_bug.cgi?id=3605
				 */
				sciprint(_("Please check if TCL_LIBRARY has been set.\n"));
			}else{
				sciprint(_("Reason: %s:\n"),getTclInterp()->result);			
			}
		}
		releaseTclInterp();

		if ( Tk_Init(getTclInterp()) == TCL_ERROR)
		{
			Scierror(999,_("Tcl Error: Error during the TK initialization (Tk_Init).\n"));
			if (IsFromJava()){
				/* Same comment as upper about TCL_LIBRARY */
				sciprint(_("Please check if TK_LIBRARY has been set.\n"));

			}else{
				sciprint(_("Reason: %s:\n"),getTclInterp()->result);
			}
			tkStarted=FALSE;
		}

		releaseTclInterp();
		sprintf(MyCommand, "set SciPath \"%s\";",SciPath);

		if ( Tcl_Eval(getTclInterp(),MyCommand) == TCL_ERROR  )
		{
			Scierror(999,_("Tcl Error: Error during the Scilab/Tcl init process. Could not set SciPath: %s\n"),getTclInterp()->result);
		}

		releaseTclInterp();
		Tcl_CreateCommand(getTclInterp(),"ScilabEval",TCL_EvalScilabCmd,(ClientData)1,NULL);
		releaseTclInterp();
	}

	if (TKmainWindow == NULL && tkStarted)
	{
		TKmainWindow = Tk_MainWindow(getTclInterp());
		releaseTclInterp();
		Tk_GeometryRequest(TKmainWindow,2,2);
		if ( Tcl_EvalFile(getTclInterp(),TkScriptpath) == TCL_ERROR  )
		{
			Scierror(999,_("Tcl Error: Error during the Scilab/TK init process. Error while loading %s: %s\n"),TkScriptpath, getTclInterp()->result);
		}
		releaseTclInterp();
	}


	if (SciPath) {FREE(SciPath);SciPath=NULL;}

	__LockSignal(&InterpReadyLock);
	// Signal TclInterpreter init is now over.
	__Signal(&InterpReady);
	__UnLockSignal(&InterpReadyLock);

	// This start a periodic and endless call to "update"
	// TCL command. This causes any TCL application to start
	// and run as if it's in the main program thread.
	startTclLoop();
	return(0);

}
/*--------------------------------------------------------------------------*/
int OpenTCLsci(void)
{
	__InitSignalLock(&InterpReadyLock);
	__InitSignal(&InterpReady);
	// Open TCL interpreter in a separated thread.
	// Allows all Tcl application not to freeze nor decrease Scilab speed.
	// Causes also Scilab let those application live their own lifes.


	__CreateThread(&TclThread, &DaemonOpenTCLsci);
	// Wait to be sure initialisation is complete.
	__LockSignal(&InterpReadyLock);
	__Wait(&InterpReady, &InterpReadyLock);
	__UnLockSignal(&InterpReadyLock);
	return 0;
}
/*--------------------------------------------------------------------------*/
BOOL CloseTCLsci(void)
{
	if ( getScilabMode() != SCILAB_NWNI )
	{
		if (isTkStarted())
		{
			setTkStarted(FALSE);
			__Terminate(TclThread);
			__WaitThreadDie(TclThread);
			deleteTclInterp();
			TKmainWindow=NULL;
			return TRUE;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
static char *GetSciPath(void)
/* force SciPath to Unix format for compatibility (Windows) */
{
	char *PathUnix=NULL;
	char *SciPathTmp=NULL;
	int i=0;

	SciPathTmp=getSCIpath();

	if (SciPathTmp)
	{
		PathUnix = strdup(SciPathTmp);
		for (i=0;i<(int)strlen(PathUnix);i++)
		{
			if (PathUnix[i]=='\\') PathUnix[i]='/';
		}
	}
	if (SciPathTmp) {FREE(SciPathTmp);SciPathTmp=NULL;}
	return PathUnix;
}
/*--------------------------------------------------------------------------*/
BOOL isTkStarted(void)
{
	return TK_Started;
}
/*--------------------------------------------------------------------------*/
void setTkStarted(BOOL isTkSet)
{
	TK_Started = isTkSet;
}
/*--------------------------------------------------------------------------*/
