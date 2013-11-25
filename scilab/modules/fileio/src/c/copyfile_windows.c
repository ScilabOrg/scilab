/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#ifdef _MSC_VER
#include <windows.h>
#include <shellapi.h>
#include <string.h>
#include "copyfile.h"
#include "BOOL.h"
#include "charEncoding.h"
#include "SCIMALLOC.h"
#include "isdir.h"
#include "createdirectory.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
static int CopyFileFunction_windows(wchar_t *DestinationFilename, wchar_t *SourceFilename);
static int CopyDirectoryFunction_windows(wchar_t *DestinationDirectory, wchar_t *SourceDirectory);
/*--------------------------------------------------------------------------*/
int CopyFileFunction(wchar_t *DestinationFilename, wchar_t *SourceFilename)
{
    if (_wcsicmp(DestinationFilename, SourceFilename) == 0)
    {
        SetLastError(ERROR_ACCESS_DENIED);
        return 1;
    }
    return CopyFileFunction_windows(DestinationFilename, SourceFilename);
}
/*--------------------------------------------------------------------------*/
int CopyDirectoryFunction(wchar_t *DestinationDirectory, wchar_t *SourceDirectory)
{
    /* remove last file separator if it does not exists */
    if ( (SourceDirectory[wcslen(SourceDirectory) - 1] == L'\\') ||
            (SourceDirectory[wcslen(SourceDirectory) - 1] == L'/') )
    {
        SourceDirectory[wcslen(SourceDirectory) - 1] = L'\0';
    }

    /* remove last file separator if it does not exists */
    if ( (DestinationDirectory[wcslen(DestinationDirectory) - 1] == L'\\') ||
            (DestinationDirectory[wcslen(DestinationDirectory) - 1] == L'/') )
    {
        DestinationDirectory[wcslen(DestinationDirectory) - 1] = L'\0';
    }

    if (_wcsicmp(DestinationDirectory, SourceDirectory) == 0)
    {
        SetLastError(ERROR_ACCESS_DENIED);
        return 1;
    }


    return CopyDirectoryFunction_windows(DestinationDirectory, SourceDirectory);
}
/*--------------------------------------------------------------------------*/
static int CopyFileFunction_windows(wchar_t *DestinationFilename, wchar_t *SourceFilename)
{
    BOOL bFailIfExists = FALSE;
    if (!CopyFileW(SourceFilename, DestinationFilename, bFailIfExists))
    {
        return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int CopyDirectoryFunction_windows(wchar_t *DestinationDirectory, wchar_t *SourceDirectory)
{
    WIN32_FIND_DATAW dir_find_data;
    wchar_t src_buffer[MAX_PATH * 2 + 1];
    wchar_t dest_buffer[MAX_PATH * 2 + 1];
    HANDLE find_handle = FindFirstFileW(DestinationDirectory, &dir_find_data);
    BOOL ans;

    if (find_handle && (find_handle != INVALID_HANDLE_VALUE))
    {
        /* destdir exists, now make sure it is a directory */
        if (dir_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            ans = 1;
        }
        else
        {
            ans = 0;
        }
        FindClose(find_handle);
    }
    else
    {
        /* Create the destdir */
        ans = CreateDirectoryW(DestinationDirectory, NULL);
    }
    FindClose(find_handle);

    if (!ans)
    {
        return 1;
    }

    /* Get ready to do some copying */
    wcscpy(src_buffer, SourceDirectory);
    wcscat(src_buffer, L"\\*");

    wcscpy(dest_buffer, DestinationDirectory);
    wcscat(dest_buffer, L"\\");

    find_handle = FindFirstFileW(src_buffer, &dir_find_data);
    if (find_handle && find_handle != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (wcscmp(dir_find_data.cFileName, L".") == 0)
            {
                continue;
            }
            if (wcscmp(dir_find_data.cFileName, L"..") == 0)
            {
                continue;
            }

            wcscpy(src_buffer + wcslen(SourceDirectory) + 1, dir_find_data.cFileName);
            wcscpy(dest_buffer + wcslen(DestinationDirectory) + 1, dir_find_data.cFileName);

            if (dir_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                ans = (CopyDirectoryFunction_windows( dest_buffer, src_buffer) == 0);
            }
            else
            {
                ans = CopyFileW(src_buffer, dest_buffer, FALSE);
            }
            if (!ans)
            {
                break;
            }
        }
        while (FindNextFileW(find_handle, &dir_find_data));
        FindClose(find_handle);
    }
    return (ans ? 0 : 1);
}
/*--------------------------------------------------------------------------*/
#endif /* #ifdef _MSC_VER */
/*--------------------------------------------------------------------------*/
