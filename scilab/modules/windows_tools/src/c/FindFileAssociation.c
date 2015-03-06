/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <shlwapi.h>
#include <stdio.h>
#include "FindFileAssociation.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "PATH_MAX.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"shlwapi.lib") /* AssocQueryString */
/*--------------------------------------------------------------------------*/
wchar_t* FindFileAssociation (const wchar_t* ptrFindStr, const wchar_t* Extra)
{
    if (ptrFindStr)
    {
        wchar_t szDefault[PATH_MAX + FILENAME_MAX];
        DWORD ccDefault = PATH_MAX + FILENAME_MAX;

        if (ptrFindStr)
        {
            HRESULT rc = AssocQueryStringW(0, ASSOCSTR_EXECUTABLE, ptrFindStr, Extra, szDefault, &ccDefault);
            if (ccDefault)
            {
                if (rc == S_OK)
                {
                    return os_wcsdup(szDefault);
                }
            }
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/

