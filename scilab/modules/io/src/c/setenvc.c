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

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "setenvc.h"
#include "../../../core/src/c/dynamic_tclsci.h"
#include "MALLOC.h" /* MALLOC */
#include "charEncoding.h"
#include "os_swprintf.h"
/*--------------------------------------------------------------------------*/
BOOL setenvc(char *stringIn,char *valueIn)
{
	BOOL ret = TRUE;
	int len_env = 0;
#ifdef _MSC_VER
	{
		/*
		On Windows :
		each process has two copies of the environment variables,
		one managed by the OS and one managed by the C library. We set
		the value in both locations, so that other software that looks in
		one place or the other is guaranteed to see the value.
		*/

		#define ENV_FORMAT L"%s=%s"
		wchar_t* wstringIn = to_wide_string(stringIn);
		wchar_t* wvalueIn = to_wide_string(valueIn);

		if (SetEnvironmentVariableW(wstringIn,wvalueIn) == 0)
		{
			ret = FALSE;
		}

		len_env = (int) (wcslen(wstringIn) + wcslen(wvalueIn) + wcslen(ENV_FORMAT)) + 1;
		if (len_env < _MAX_ENV)
		{
			wchar_t *env = (wchar_t*) MALLOC(len_env * sizeof(wchar_t));
			if (env)
			{
				os_swprintf(env, len_env, L"%s=%s", wstringIn, wvalueIn);
				if(_wputenv(env))
				{
					ret = FALSE;
				}

				FREE(env);env = NULL;
			}
		}
		else ret = FALSE;
	}
#else
	/* linux and Mac OS X */
	/* setenv() function is strongly preferred to putenv() */
	/* http://developer.apple.com/documentation/Darwin/Reference/ManPages/man3/setenv.3.html */
	
	#ifndef _MAX_ENV 
		#define _MAX_ENV 32767
	#endif

	len_env = (int)(strlen(stringIn) + strlen(valueIn) + 1);
	if (len_env < _MAX_ENV)
	{
		if ( setenv(stringIn, valueIn, 1) ) 
		{
			ret = FALSE;
		}
	}
	else ret = FALSE;

#endif

	if(ret)
	{
		dynamic_setenvtcl(stringIn,valueIn);
	}

	return ret;
}
/*--------------------------------------------------------------------------*/
