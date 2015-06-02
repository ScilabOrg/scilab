/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <unknwn.h>
#include "createGUID.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
#define _OLEAUT32_
/*--------------------------------------------------------------------------*/
wchar_t *createGUID(void)
{
    GUID guid;
    wchar_t* pwstrGUID = NULL;
    wchar_t* ret = NULL;

    CoCreateGuid (&guid);
    StringFromCLSID(&guid, &pwstrGUID);

    //remove first '{' and last '}'
    pwstrGUID[wcslen(pwstrGUID) - 1] = L'\0';
    ret = os_wcsdup(pwstrGUID + 1);
    CoTaskMemFree(pwstrGUID);
    return ret;
}
/*--------------------------------------------------------------------------*/
