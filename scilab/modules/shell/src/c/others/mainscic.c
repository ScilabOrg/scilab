/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CONRET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "core_math.h"
#include "version.h"
#include "realmain.h" /* realmain */
#include "scimem.h" /* nofpex */
#include "scilabmode.h"
#include "getcommandlineargs.h"
#include "texmacs.h"
#include "x_main.h"
#include "LaunchScilabSignal.h"

#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif

/*--------------------------------------------------------------------------*/
#define MIN_STACKSIZE 180000
/*--------------------------------------------------------------------------*/
char *ProgramName;
/*--------------------------------------------------------------------------*/
int  sci_show_banner=1;
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
  int i;
  int  no_startup_flag=0;
  int  memory = MIN_STACKSIZE;

  char * initial_script = NULL;
  InitScriptType initial_script_type = SCILAB_SCRIPT;
  /* This bug only occurs under Linux 32 bits */
#if defined(linux) && defined(__i386__)
  setFPUToDouble();
#endif

  InitializeLaunchScilabSignal();

  #if (defined __GNUC__  )
		putenv ("COMPILER=gcc");
	#else
		putenv ("COMPILER=cc or another");
	#endif

#if defined(netbsd) || defined(freebsd)
/* floating point exceptions */
fpsetmask(0);
#endif

  ProgramName = argv[0];
#ifdef WITHOUT_GUI
  setScilabMode(SCILAB_NWNI);
#else
  setScilabMode(SCILAB_STD);
#endif

  setCommandLineArgs(argv, argc);


  /* scanning options */
  for ( i=0 ; i < argc ; i++)
  {
      if ( strcmp(argv[i],"-nw") == 0)
      {
      	setScilabMode(SCILAB_NW);
      }
      else if ( strcmp(argv[i],"-nwni") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      }
      else if ( strcmp(argv[i],"-display") == 0 || strcmp(argv[i],"-d") == 0)
      {
		  /* @TODO Buffer overflow here */
		  char dpy[128];
		  sprintf(dpy,"DISPLAY=%s",argv[++i]);
		  putenv(dpy);
      }
      else if ( strcmp(argv[i],"-l") == 0)
      {
		  /* @TODO Buffer overflow here */
		  char lang[128];
		  char *argLang=strdup(argv[++i]);

		  /* Export the locale. This is going to be used by setlanguage("") in
			 modules/localization/src/c/InitializeLocalization.c */
		  if (strcmp(argLang,"en")==0) {/* backward compatiblity en => en_US */
			  setenvc("LANG","en_US"); 
		  }else{
			  if (strcmp(argLang,"fr")==0) { /* backward compatiblity fr => fr_FR */
				  setenvc("LANG","fr_FR");
			  }else{
				  setenvc("LANG",argLang);
			  }
		  }
      }
      else if ( strcmp(argv[i],"-nb") == 0)  { sci_show_banner = 0; }
      else if ( strcmp(argv[i],"-ns") == 0)  { no_startup_flag = 1;}
      else if ( strcmp(argv[i],"-mem") == 0) { i++; memory = Max(atoi(argv[i]),MIN_STACKSIZE );}
      else if ( strcmp(argv[i],"-f") == 0)   { initial_script = argv[++i];}
      else if ( strcmp(argv[i],"-e") == 0)
      {
		  initial_script = argv[++i];
		  initial_script_type = SCILAB_CODE;
      }
      else if ( strcmp(argv[i],"--texmacs") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      	settexmacs();
      }
      else if ( strcmp(argv[i],"-nogui") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      }
      else if ( strcmp(argv[i],"-version") == 0) {disp_scilab_version();exit(1);}
    }

#ifndef WITH_GUI
  if(getScilabMode() != SCILAB_NWNI)
  {
    fprintf(stderr, "Scilab was compiled without its GUI. Run scilab with the -nwni option.\n");
    exit(1);
  }
#endif

  return realmain(no_startup_flag,initial_script,initial_script_type,memory);
}
/*--------------------------------------------------------------------------*/
