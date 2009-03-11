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

#ifdef _MSC_VER
#include <wchar.h>

/**
* convert a UTF string to wide char string
* @param[in] UTF string
* @return wide char string converted
*/
wchar_t *to_wide_string(char *_UTFStr);

#endif 

char* readNextUTFChar(char* utfstream,int* size);


#endif /* __CHARENCODING_H__ */

