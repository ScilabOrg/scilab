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
#ifdef _MSC_VER
	#include <wchar.h>
#endif 
#include <stdlib.h>
#include <stdio.h>
#include "charEncoding.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static int wide_string_size(wchar_t *_wide)
{
	DWORD size;

	if ((wchar_t *)NULL == _wide) return 0;
	size = wcslen(_wide)+1;
	return size*2;
}
/*--------------------------------------------------------------------------*/
char *wide_string_to_UTF8(wchar_t *_wide)
{
	DWORD len = 0;
	DWORD size = 0;
	char *buf = NULL;

	if ((wchar_t *)NULL == _wide) return (char *)NULL;
	
	len = wide_string_size(_wide);

	size = WideCharToMultiByte(CP_UTF8, 0, _wide, len, NULL, 0, NULL, 0);
	if (size == 0) return (char *)NULL;
	buf = (char*)MALLOC(sizeof(char)*size);
	if (buf)
	{
		size = WideCharToMultiByte(CP_UTF8, 0, _wide, len, buf, size, NULL, 0);
		if (size == 0) 
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
	_buf = MALLOC(nwide * sizeof(WCHAR));
	if(_buf == NULL) return NULL;
	if(MultiByteToWideChar(CP_UTF8, 0, _UTFStr, -1, _buf, nwide) == 0) 
	{
		FREE(_buf);
		return NULL;
	}
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
