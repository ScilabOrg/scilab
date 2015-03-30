/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#else
#include <Windows.h>
#include <stdio.h>
#endif
#include "charEncoding.h"
#include "MALLOC.h"
#include "createdirectory.h"
#include "isdir.h"
#include "splitpath.h"
#include "scicurdir.h"
/*--------------------------------------------------------------------------*/
#define DIRMODE 0777
/*--------------------------------------------------------------------------*/
BOOL createdirectory(const char *path)
{
    BOOL bOK = FALSE;

    if (path)
    {
        if  (!isdir(path))
        {
#ifndef _MSC_VER
            if (mkdir(path, DIRMODE) == 0)
            {
                bOK = TRUE;
            }
#else
            wchar_t *widePath = to_wide_string((char*)path);
            if (widePath)
            {
                bOK = createdirectoryW(widePath);
                FREE(widePath);
                widePath = NULL;
            }
#endif
        }
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL createdirectoryW(const wchar_t *pathW)
{
    BOOL bOK = FALSE;
    if (pathW)
    {
        if (!isdirW(pathW))
        {
#ifndef _MSC_VER
            char *path = wide_string_to_UTF8(pathW);
            if (path)
            {
                bOK = createdirectory(path);
                FREE(path);
                path = NULL;
            }
#else
            wchar_t path_out[MAX_PATH];
            wchar_t drv[MAX_PATH];
            wchar_t dir[MAX_PATH];
            splitpathW(pathW, TRUE, drv, dir, NULL, NULL);

            wcscpy(path_out, drv);
            wcscat(path_out, dir);

            //if there is no path_out, get current dir as reference.
            if (wcslen(path_out) == 0)
            {
                int err = 0;
                wchar_t * cur = NULL;
                if (wcslen(pathW)  == 0)
                {
                    return FALSE;
                }

                cur = scigetcwdW(&err);
                wcscpy(path_out, cur);
                FREE(cur);
            }

            if (CreateDirectoryExW(path_out, pathW, NULL))
            {
                bOK = TRUE;
            }
#endif
        }
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/