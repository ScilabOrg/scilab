/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
#include "matscicos.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if WIN32
#include <Windows.h>
#include "../os_specific/win_mem_alloc.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
extern int intendscicosim _PARAMS((char *fname, unsigned long l));
extern int inttimescicos _PARAMS((char *fname,unsigned long l));
extern int intduplicate _PARAMS((char *fname,unsigned long l));
extern int intdiffobjs _PARAMS((char *fname,unsigned long l));
extern int intxproperty _PARAMS((char *fname,unsigned long l));
extern int intphasesim _PARAMS((char *fname,unsigned long l));
extern int intsetxproperty _PARAMS((char *fname,unsigned long l));
extern int intsetblockerror _PARAMS((char *fname,unsigned long l));
extern int inttree2 _PARAMS((char *fname,unsigned long l));
extern int inttree3 _PARAMS((char *fname,unsigned long l));
extern int inttree4 _PARAMS((char *fname,unsigned long l));
extern int intscicosimc _PARAMS((char *fname, unsigned long l));
extern int intgetscicosvarsc _PARAMS((char *fname, unsigned long l));
extern int intcurblkc _PARAMS((char *fname, unsigned long l));
extern int intbuildouttb _PARAMS((char *fname, unsigned long l));
extern int intpermutobj_c _PARAMS((char *fname,unsigned long l));
extern int intscixstringb _PARAMS((char *fname,unsigned long l));
/*-----------------------------------------------------------------------------------*/ 
static intcscicosTable Tab[]={
  {inttimescicos,"scicos_time"},
  {intduplicate,"duplicate"},
  {intdiffobjs,"diffobjs"},
  {intxproperty,"pointer_xproperty"},
  {intphasesim,"phase_simulation"},
  {intsetxproperty,"set_xproperty"},
  {intsetblockerror,"set_blockerror"},
  {inttree2,"ctree2"},
  {inttree3,"ctree3"},
  {inttree4,"ctree4"},
  {intscicosimc,"scicosim"},
  {intgetscicosvarsc,"getscicosvars"},
  {intcurblkc,"curblockc"},
  {intbuildouttb,"buildouttb"},
  {intpermutobj_c,"permutobj"},
  {intscixstringb,"xstringb2"},
  {intendscicosim,"end_scicosim"},
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(intcscicos)()
{  
	Rhs = Max(0, Rhs);
	#if WIN32
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
		#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		#endif
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif

	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
