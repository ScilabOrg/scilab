/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "getshortpathname.h"
#include "MALLOC.h"
#include "charEncoding.h"
#include "os_strdup.h"
#include "os_wcsdup.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
	#ifndef MAX_PATH_SHORT
		#define MAX_PATH_SHORT 260
	#endif
#endif
/*--------------------------------------------------------------------------*/
int C2F(getshortpathname)(char *pathname,int *len)
{
	if (pathname)
	{
		BOOL bConvert = FALSE;
		char *result = NULL;
		pathname[*len] = 0;
		result = getshortpathname(pathname,&bConvert);
		if (result)
		{
			strcpy(pathname, result);
			*len = (int)strlen(result);
			FREE(result);
			result = NULL;
			return 1;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
char *getshortpathname(char *longpathname,BOOL *convertok)
{
	char *ShortName = NULL;

	if (longpathname)
	{
		#ifdef _MSC_VER
		/* first we try to call to know path length */
		wchar_t *ptwlongpathname = to_wide_string(longpathname);
		wchar_t *ptwShortName = NULL;
		int length = GetShortPathNameW(ptwlongpathname, NULL, 0);

		if (length <= 0 ) length = MAX_PATH_SHORT;

		ptwShortName = (wchar_t*)MALLOC((length + 1)*sizeof(wchar_t));

		if (ptwShortName)
		{
			/* second converts path */
			if ( GetShortPathNameW(ptwlongpathname, ptwShortName, length) )
			{
				ShortName = wide_string_to_UTF8(ptwShortName);
				*convertok = TRUE;
			}
			else
			{
				/* FAILED */
				ShortName = os_strdup(longpathname);
				*convertok = FALSE;
			}
			if (ptwShortName) {FREE(ptwShortName);ptwShortName = NULL;}
		}
		else
		{
			/* FAILED */
			ShortName = os_strdup(longpathname);
			*convertok = FALSE;
		}
		if (ptwlongpathname) { FREE(ptwlongpathname); ptwlongpathname = NULL;}
		#else
		/* Linux */
		int length = (int)strlen(longpathname) + 1;
		ShortName = (char*)MALLOC((length) * sizeof(char));
		if (ShortName) strcpy(ShortName, longpathname);
		*convertok = FALSE;
		#endif
	}
	else
	{
		/* FAILED */
		*convertok = FALSE;
	}
	return ShortName;
}
/*--------------------------------------------------------------------------*/
wchar_t* getshortpathnameW(wchar_t* _pwstLongPathName, BOOL* _pbOK)
{
    wchar_t* pwstOutput = NULL;
    if(_pwstLongPathName)
    {
#ifdef _MSC_VER
        int iLen = GetShortPathNameW(_pwstLongPathName, NULL, 0);

        if(iLen <= 0)
        {
            iLen = MAX_PATH_SHORT;
        }

        pwstOutput = (wchar_t*)MALLOC((iLen + 1) * sizeof(wchar_t));

        if(pwstOutput)
        {
            /* second converts path */
            if(GetShortPathNameW(_pwstLongPathName, pwstOutput, iLen))
            {
                *_pbOK = TRUE;
            }
            else
            {
                FREE(pwstOutput);
                pwstOutput = os_wcsdup(_pwstLongPathName);
                *_pbOK = FALSE;
            }
        }
        else
        {
            /* FAILED */
            pwstOutput = os_wcsdup(_pwstLongPathName);
            *_pbOK = FALSE;
        }
#else
		/* Linux and MacOS*/
        pwstOutput = os_wcsdup(_pwstLongPathName);
		*_pbOK = FALSE;
#endif
    }
    else
    {
        *_pbOK = FALSE;
    }
    return pwstOutput;
}
/*--------------------------------------------------------------------------*/
