/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __OS_STRING_H__
#define __OS_STRING_H__
#include <wchar.h>
#include <string.h>
// Windows
#ifdef _MSC_VER
#define os_wcsdup       _wcsdup
#define os_strdup       _strdup
#define os_swprintf     swprintf_s
#define os_sprintf      sprintf_s
#define os_wcsicmp      _wcsicmp
#define stricmp         _stricmp
#define strnicmp        _strnicmp
#define wcsicmp         _wcsicmp
#define wcsnicmp        _wcsnicmp
#define os_strcpy       strcpy_s
#define os_splitpath    _splitpath_s
#define os_wsplitpath   _wsplitpath_s
#define os_makepath     _makepath_s
#endif

// Linux
#ifdef __linux__
#define os_wcsdup       wcsdup
#define os_strdup       strdup
#define os_swprintf     swprintf
#define os_sprintf      sprintf
#define os_wcsicmp      wcscasecmp
#define stricmp         strcasecmp
#define strnicmp        strncasecmp
#define wcsicmp         wcscasecmp
#define wcsnicmp        wcsncasecmp
#define os_strcpy       strcpy
#define os_splitpath    _splitpath
#define os_wsplitpath   _wsplitpath
#define os_makepath     _makepath
#endif

// MacOS X
#ifdef __APPLE__
wchar_t *macOSwcsdup(const wchar_t *_pwcsSource);
#define os_wcsdup       macOSwcsdup
#define os_strdup       strdup
#define os_swprintf     swprintf
#define os_sprintf      sprintf

int macOSwcscasecmp(const wchar_t *_pwcsS1, const wchar_t *_pwcsS2);
#define os_wcsicmp      macOSwcscasecmp
#define stricmp         strcasecmp
#define strnicmp        strncasecmp
#define wcsicmp         wcscasecmp
#define wcsnicmp        wcsncasecmp
#define os_strcpy       strcpy
#define os_splitpath    _splitpath
#define os_wsplitpath   _wsplitpath
#define os_makepath     _makepath
#endif

#endif /* !__OS_STRING_H__ */