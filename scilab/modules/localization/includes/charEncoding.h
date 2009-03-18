/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#ifndef __CHARENCODING_H__
#define __CHARENCODING_H__

#include <wchar.h>
#include "MALLOC.h"

#ifdef _MSC_VER
#define wcfopen(fp, x,y) \
{\
	wchar_t* wfilename = NULL;\
	wchar_t* wmode = NULL;\
	wfilename = to_wide_string(x);\
	wmode = to_wide_string(y);\
	if(wfilename == NULL || wmode == NULL){fp = 0;}\
	fp = _wfopen(wfilename, wmode);\
	if(wfilename != NULL){FREE(wfilename);}\
	if(wmode != NULL){FREE(wmode);} \
}
#else
#define wcfopen(fp, x,y) \
	{\
		fp = fopen(filename, mode);\
	}
#endif


/**
* convert a UTF string to wide char string
* @param[in] UTF string
* @return wide char string converted
*/
wchar_t *to_wide_string(char *_UTFStr);

/**
* convert a wide char string to UTF-8
* @param[in] wide char string
* @return UTF string converted
*/
char *wide_string_to_UTF8(wchar_t *_wide);

//#endif

char* readNextUTFChar(char* utfstream,int* size);



/*file management with UTF filename*/

//FILE * wcfopen(char *filename, char* mode);


#endif /* __CHARENCODING_H__ */

