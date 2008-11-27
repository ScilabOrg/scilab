/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "CallScilab.h"
#include "MALLOC.h"
#include "../../../graphics/includes/WindowList.h"
#include "../../../graphics/includes/GetProperty.h"
#include "../../../io/includes/setenvc.h"
#include "setgetSCIpath.h"
#include "fromc.h"
#include "sciquit.h" /* ExitScilab */
#include "tmpdir.h"
#include "fromc.h"
#include "inisci-c.h"
#include "scirun.h"
#include "scilabmode.h"
#include "dynamic_menus.h"
#include "scilabDefaults.h"
#include "TerminateCore.h"
#ifdef _MSC_VER
#include "../../../windows_tools/src/c/scilab_windows/SetScilabEnvironmentVariables.h"
#include "../../../windows_tools/src/c/scilab_windows/getScilabDirectory.h"
#include "strdup_windows.h"
#endif
#include "localization.h"

#ifdef _MSC_VER
#define putenv _putenv
#endif
/*--------------------------------------------------------------------------*/
static BOOL StartScilabIsOK = FALSE;
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static void SetSciEnv(void)
{
  char *ScilabDirectory=NULL;

  ScilabDirectory = getScilabDirectory(TRUE);

  if (ScilabDirectory == NULL)
  {
	MessageBox (NULL, "getScilabDirectory()", gettext("Error") , MB_ICONSTOP | MB_OK);
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
BOOL StartScilab(char *SCIpath,char *ScilabStartup,int *Stacksize)
{
	#define FORMAT_SCRIPT_STARTUP "exec(\"%s\",-1);quit;"
	char *ScilabStartupUsed = NULL;
	char *InitStringToScilab = NULL;
	int StacksizeUsed = 0;
	int lengthStringToScilab = 0;

	static int iflag = -1, ierr = 0;

	if (StartScilabIsOK) return FALSE;

	SetFromCToON();

	if (SCIpath == NULL)
	{
		#ifdef _MSC_VER
			SetSciEnv();
		#else
		{
			char env[2048];
			setSCIpath(SCIpath);
			sprintf(env,"SCI=%s",SCIpath);
			putenv(env);
		}
		#endif
	}
	else
	{
		char env[2048];
		setSCIpath(SCIpath);
		sprintf(env,"SCI=%s",SCIpath);
		putenv(env);
	}

	if (ScilabStartup == NULL)
	{
		ScilabStartupUsed = strdup(DEFAULTSCILABSTARTUP);
	}
	else
	{
		ScilabStartupUsed = strdup(ScilabStartup);
	}

	if (Stacksize==NULL)
	{
		StacksizeUsed = DEFAULTSTACKSIZE;
	}
	else
	{
		StacksizeUsed = *Stacksize;
	}

	/* creates TMPDIR */
	C2F(settmpdir)();

	/* Scilab Initialization */
	C2F(inisci)(&iflag,&StacksizeUsed,&ierr);
	if ( ierr > 0 ) return FALSE;

	lengthStringToScilab = (int)(strlen(FORMAT_SCRIPT_STARTUP) + strlen(ScilabStartupUsed + 1));
	InitStringToScilab = (char*)MALLOC(lengthStringToScilab*sizeof(char));
	sprintf(InitStringToScilab,FORMAT_SCRIPT_STARTUP,ScilabStartupUsed);

	C2F(scirun)(InitStringToScilab,(long int)strlen(InitStringToScilab));

	if (ScilabStartupUsed) {FREE(ScilabStartupUsed);ScilabStartupUsed=NULL;}
	if (InitStringToScilab) {FREE(InitStringToScilab);InitStringToScilab=NULL;}

	StartScilabIsOK = TRUE;

	return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL TerminateScilab(char *ScilabQuit)
{
	if (StartScilabIsOK)
	{
		if (getScilabMode() != SCILAB_NWNI)
		{
			ExitScilab();
		}
		else
		{
			TerminateCorePart2();
		}
		StartScilabIsOK = FALSE;
		return TRUE;
	}

	return FALSE;
}
/*--------------------------------------------------------------------------*/
/*
  Where is this function called ????
  What's that shit...
*/
void ScilabDoOneEvent(void)
{
	if ( getScilabMode() != SCILAB_NWNI )
	{
	  //ScilabEventsLoop();

		while(ismenu()==1 )
		{
			C2F(scirun)("quit;",(int)strlen("quit;"));
		}
	}
}
/*--------------------------------------------------------------------------*/
int ScilabHaveAGraph(void)
{
  return sciHasFigures();
}
/*--------------------------------------------------------------------------*/
