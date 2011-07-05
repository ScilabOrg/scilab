/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
#include <Windows.h>
#include "charEncoding.h"
#else
#include <sys/types.h>
#include <dirent.h>
#include "PATH_MAX.h"
#endif

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "fileutils.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static unsigned long* fileproperties_Windows(char *path);
#else
static unsigned long* fileproperties_Others(char *path);
#endif
/*--------------------------------------------------------------------------*/
unsigned long* getFileProperties(char *path)
{
    unsigned long* properties = NULL;

#ifdef _MSC_VER
    properties = fileproperties_Windows(path);
#else
    properties = fileproperties_Others(path);
#endif

    if (properties == NULL)
    {
        properties = MALLOC(0);
    }

    return properties;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
int isEmptyDirectory(char * dirName)
{
    wchar_t *wcpath = NULL;
    HANDLE hFile;
    WIN32_FIND_DATAW FileInformation;
    int ret = 1;

    wcpath = to_wide_string(dirName);

    hFile = FindFirstFileW(wcpath, &FileInformation);
    FREE(wcpath);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    do
    {
        if (!wcscmp(FileInformation.cFileName, L".") || !wcscmp(FileInformation.cFileName, L".."))
        {
            continue;
        }

        ret = 0;
        break;
    } while (FindNextFileW(hFile, &FileInformation) == TRUE);

    FindClose(hFile);

    return ret;
}
/*--------------------------------------------------------------------------*/
static unsigned long* fileproperties_Windows(char *path)
{
    struct _stat buf;
    unsigned long* properties = NULL;
    wchar_t* wcpath = NULL;
    wchar_t DriveTemp[PATH_MAX + FILENAME_MAX + 1];

    properties = MALLOC(6 * sizeof(unsigned long));
    memset(properties, 0, 6 * sizeof(unsigned long));

    if (path == NULL)
    {
        return properties;
    }

    wcpath = to_wide_string(path);
    if (wcpath == NULL)
    {
        return properties;
    }

    swprintf(DriveTemp, wcslen(wcpath)+1, L"%s", wcpath);
    if (DriveTemp[wcslen(DriveTemp)-1] == L'/' || DriveTemp[wcslen(DriveTemp)-1] == L'\\')
    {
        DriveTemp[wcslen(DriveTemp)-1] = L'\0';
    }

    FREE(wcpath);
    wcpath = NULL;

    if (_wstat(DriveTemp, &buf))
    {
        return properties;
    }

    mode = buf.st_mode;

    properties[0] = (mode & S_IFMT) == S_IFREG; // isFile
    properties[1] = (mode & S_IFMT) == S_IFDIR; // isDirectory

    if (properties[0] || properties[1])
    {
	properties[2] = 1000 * (unsigned long) buf.st_mtime; // lastModified
	properties[3] = (unsigned long) buf.st_size; // length
	properties[4] = access(path, R_OK) == 0; // canRead
	properties[5] = access(path, W_OK) == 0; // canWrite
    }

    return properties;
}
#else
/*--------------------------------------------------------------------------*/
int isEmptyDirectory(char * dirName)
{
    DIR *dir = NULL;
#ifdef __APPLE__
    struct dirent *ptr;
    struct dirent *result;
#else
    struct dirent64 *ptr;
    struct dirent64 *result;
#endif
    int ret = 1;

    dir = opendir(dirName);
    if (dir == NULL)
    {
        return 0;
    }

#ifdef __APPLE__
    ptr = MALLOC(sizeof(struct dirent) + (PATH_MAX + 1));
#else
    ptr = MALLOC(sizeof(struct dirent64) + (PATH_MAX + 1));
#endif
    if (ptr == NULL)
    {
        closedir(dir);
        return 0;
    }

#ifdef __APPLE__
    while ((readdir_r(dir, ptr, &result) == 0)  && (result != NULL))
#else
        while ((readdir64_r(dir, ptr, &result) == 0)  && (result != NULL))
#endif
        {
            if (!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, ".."))
            {
                continue;
            }

            ret = 0;
            break;
        }

    FREE(ptr);
    closedir(dir);

    return ret;
}
/*--------------------------------------------------------------------------*/
static unsigned long* fileproperties_Others(char *path)
{
    unsigned long* properties = NULL;
    struct stat buf;
    int mode;

    properties = MALLOC(6 * sizeof(unsigned long));
    memset(properties, 0, 6 * sizeof(unsigned long));

    if (path == NULL)
    {
	return properties;
    }

    if (stat(path, &buf))
    {
        return properties;
    }

    mode = buf.st_mode;

    properties[0] = (mode & S_IFMT) == S_IFREG; // isFile
    properties[1] = (mode & S_IFMT) == S_IFDIR; // isDirectory

    if (properties[0] || properties[1])
    {
	properties[2] = 1000 * (unsigned long) buf.st_mtime; // lastModified
	properties[3] = (unsigned long) buf.st_size; // length
	properties[4] = access(path, R_OK) == 0; // canRead
	properties[5] = access(path, W_OK) == 0; // canWrite
    }

    return properties;
}
#endif
