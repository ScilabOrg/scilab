/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - Yung-Jang Lee
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
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

		memset (&ps, '\0', sizeof (ps));
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

		mbsrtowcs(_buf, (const char**)&psz, strlen(psz), NULL);
		return _buf;
}
#endif
/*--------------------------------------------------------------------------*/
char* readNextUTFChar(char* utfstream,int* size)
{
	static char UTFChar[5]; /**UTF char. at most 4 bytes*/
	unsigned char charcode=(unsigned)*utfstream;
	/** UTF-8 format: ref. http://en.wikipedia.org/wiki/UTF-8/ */
	if(charcode > 193 && charcode <= 223 )
	{ /* twi bytes UTF-8 */
		UTFChar[0]=*utfstream;
		UTFChar[1]=*(utfstream+1);
		UTFChar[2]='\0';
		*size=2;
	}
	else if(charcode > 223 && charcode <= 239 )
	{/* three bytes UTF-8*/
		UTFChar[0]=*utfstream;
		UTFChar[1]=*(utfstream+1);
		UTFChar[2]=*(utfstream+2);;
		UTFChar[3]='\0';
		*size=3;
	}
	else if(charcode > 239 && charcode < 245 )
	{/* four bytes UTF-8*/
		UTFChar[0]=*utfstream;
		UTFChar[1]=*(utfstream+1);
		UTFChar[2]=*(utfstream+2);
		UTFChar[3]=*(utfstream+3);
		UTFChar[4]='\0';
		*size=4;
	}
	else
	{
		UTFChar[0]=*utfstream;
		UTFChar[1]='\0';
		*size=1;
	}
	return UTFChar;
}

	/*  TEST UTF Tonio & Allan*/
/*	{
		wchar_t *psz = NULL;
		size_t pszLen = 0;
		char *pfile = filename;
		size_t iOffset = 0;

		mbstate_t ps;
		memset (&ps, '\0', sizeof (ps));

		pszLen = mbsrtowcs(NULL, (const char**)&pfile, 0, &ps) + 1;

		psz = (wchar_t*)MALLOC(pszLen * sizeof(wchar_t));

		iOffset = mbsrtowcs(psz, (const char**)&pfile, strlen(filename), &ps);

		printf("UTF-8 -> UTF-16\n");
		printf("filename(%d) : %s\n", (int)strlen(filename), filename);
		printf("psz(%d) : %ls\n", (int)iOffset, psz);
		printf("wcslen(psz) : (%d)\n\n", (int)wcslen(psz));

		{
			size_t iCharLen = 0;
			wchar_t *pwstr = psz;
			char* pchar = NULL;

			int iMaxLen = wcslen(psz) * 4;
			pchar = (char*)MALLOC((iMaxLen + 1) * sizeof(char));

			iCharLen = wcstombs (pchar, pwstr, iMaxLen);

			printf("UTF-16 -> UTF-8\n");
			printf("psz(%d) : %ls\n", (int)wcslen(psz), psz);
			printf("pchar(%d) : %s\n", (int)iCharLen, pchar);
			printf("strlen(pchar) : (%d)\n\n\n", (int)strlen(pchar));

			FREE(pchar);
		}

		FREE(psz);
	}
*/