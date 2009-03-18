/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - Yung-Jang Lee
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS , Allan CORNET
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
#include <stdio.h>
#include <string.h>
#include "charEncoding.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
char *wide_string_to_UTF8(wchar_t *_wide)
{
	DWORD size = 0;
	char *buf = NULL;

	if ((wchar_t *)NULL == _wide) return (char *)NULL;
	size = WideCharToMultiByte(CP_UTF8, 0, _wide, -1, NULL, 0, NULL, 0);
	if (size == 0) return (char *)NULL;
	size += 1;
	buf = (char*)MALLOC(sizeof(char)*size);
	if (buf)
	{
		WideCharToMultiByte(CP_UTF8, 0, _wide, -1, buf, size, NULL, 0);
		if (size <= 0)
		{
			FREE(buf);
			return (char *)NULL;
		}
	}
	return buf;
}
/*--------------------------------------------------------------------------*/
wchar_t *to_wide_string(char *_UTFStr)
{
	int nwide = 0;
	wchar_t *_buf = NULL;

	if(_UTFStr == NULL) return NULL;
	nwide = MultiByteToWideChar(CP_UTF8, 0, _UTFStr, -1, NULL, 0);
	if(nwide == 0) return NULL;
	_buf = MALLOC(nwide * sizeof(wchar_t));
	if(_buf == NULL) return NULL;
	if(MultiByteToWideChar(CP_UTF8, 0, _UTFStr, -1, _buf, nwide) == 0)
	{
		FREE(_buf);
		return NULL;
	}
	return _buf;
}

int wcstat(char* filename, struct _stat *st)
{
	int stat_result = 0;
	wchar_t *wfilename = to_wide_string(filename);
	stat_result = _wstat(wfilename, st);
	FREE(wfilename);
	return stat_result;
}
#else //Linux check for MAC OS X
char *wide_string_to_UTF8(wchar_t *_wide)
{
	size_t iCharLen = 0;
	wchar_t *pwstr = _wide;
	char* pchar = NULL;

	int iMaxLen = (int)wcslen(_wide) * 4; //MBSC Max is 4 bytes by char
	pchar = (char*)MALLOC((iMaxLen + 1) * sizeof(char));
	if(pchar == NULL)
	{
		return NULL;
	}

	iCharLen = wcstombs (pchar, pwstr, iMaxLen);
	if(iCharLen == 0)
	{
		FREE(pchar);
		return NULL;
	}
	return pchar;
}

wchar_t *to_wide_string(char *_UTFStr)
{
		wchar_t *_buf = NULL;
		size_t pszLen = 0;
		char *psz = _UTFStr;
		mbstate_t ps;

		memset (&ps, 0x00, sizeof(ps));
		pszLen = mbsrtowcs(NULL, (const char**)&psz, 0, NULL) + 1;

		if(pszLen == 0) // -1 + 1 = 0
		{
			return NULL;
		}

		_buf = (wchar_t*)MALLOC(pszLen * sizeof(wchar_t));
		if(_buf == NULL)
		{
			return NULL;
		}

		mbsrtowcs(_buf, (const char**)&psz, (int)strlen(psz), NULL);
		return _buf;
}

int wcstat(char* filename, struct stat *st)
{
	return stat(filename, st);
}

#endif
/*--------------------------------------------------------------------------*/
