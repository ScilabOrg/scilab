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

#include <stdlib.h>

#include "configvariable.hxx"

#include "string.hxx"
#include "context.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "sci_path.h"
#include "os_wcsdup.h"
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "setenvc.h"
#include "getenvc.h"
#include "setenvvar.h"
}

using namespace std;
char *getSCI(void)
{
    return wide_string_to_UTF8(ConfigVariable::getSCIPath().c_str());
}

/*--------------------------------------------------------------------------*/
wchar_t *getSCIW(void)
{
    return os_wcsdup(ConfigVariable::getSCIPath().c_str());
}

/*--------------------------------------------------------------------------*/
void setSCIW(const wchar_t* _sci_path)
{
    char* pstPath = wide_string_to_UTF8(_sci_path);
    setSCI(pstPath);
    FREE(pstPath);
}
/*--------------------------------------------------------------------------*/
void setSCI(const char* _sci_path)
{
    //
    char *ShortPath = NULL;
    char *pstSlash = new char[strlen(_sci_path) + 1];
    bool bConvertOK = false;
    ShortPath = getshortpathname(_sci_path, &bConvertOK);
    AntislashToSlash(ShortPath, pstSlash);


    //SCI
    wchar_t* pwstSCI = to_wide_string(pstSlash);
    types::String *pSSCI = new types::String(pwstSCI);
    symbol::Context::getInstance()->put(symbol::Symbol(L"SCI"), pSSCI);

    //WSCI
    wchar_t* pwstWSCI = NULL;
#ifdef _MSC_VER
    char *pstBackSlash = NULL;
    pstBackSlash = new char[strlen(_sci_path) + 1];
    SlashToAntislash(_sci_path, pstBackSlash);
    pwstWSCI = to_wide_string(pstBackSlash);
    types::String *pSWSCI = new types::String(pwstWSCI);
    symbol::Context::getInstance()->put(symbol::Symbol(L"WSCI"), pSWSCI);
    delete[] pstBackSlash;
#else
    pwstWSCI = to_wide_string(_sci_path);
#endif

    wstring wst(pwstWSCI);
    ConfigVariable::setSCIPath(wst);

    FREE(pwstWSCI);
    FREE(pwstSCI);
    if (pstSlash)
    {
        delete[] pstSlash;
    }
    if (ShortPath)
    {
        delete[] ShortPath;
    }
}
/*--------------------------------------------------------------------------*/
void putenvSCIW(const wchar_t* _sci_path)
{
    char* pstTemp = wide_string_to_UTF8(_sci_path);
    putenvSCI(pstTemp);
    FREE(pstTemp);
    return;
}
/*--------------------------------------------------------------------------*/
void putenvSCI(const char* _sci_path)
{
    char *ShortPath = NULL;
    char *CopyOfDefaultPath = NULL;

    CopyOfDefaultPath = new char[strlen(_sci_path) + 1];
    if (CopyOfDefaultPath)
    {
        /* to be sure that it's unix 8.3 format */
        /* c:/progra~1/scilab-5.0 */
        bool bConvertOK = false;
        ShortPath = getshortpathname(_sci_path, &bConvertOK);
        //GetShortPathName(_sci_path,ShortPath,PATH_MAX);
        AntislashToSlash(ShortPath, CopyOfDefaultPath);
        setenvc("SCI", ShortPath);
        if (CopyOfDefaultPath)
        {
            delete[] CopyOfDefaultPath;
            CopyOfDefaultPath = NULL;
        }

        if (ShortPath)
        {
            delete[] ShortPath;
            ShortPath = NULL;
        }
    }
    return;
}
/*--------------------------------------------------------------------------*/
wchar_t* getenvSCIW()
{
    char *SciPath = getenvSCI();
    wchar_t* pstTemp = to_wide_string(SciPath);
    delete[] SciPath;
    return pstTemp;
}
/*--------------------------------------------------------------------------*/
char* getenvSCI()
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *SciPath = new char[PATH_MAX];

    if (SciPath)
    {
        getenvc(&ierr, "SCI", SciPath, &lbuf, &iflag);

        if (ierr == 1)
        {
            return NULL;
        }
    }

    return SciPath;
}
/*--------------------------------------------------------------------------*/
wchar_t* computeSCIW()
{
    char* pstTemp = computeSCI();
    wchar_t* pstReturn = to_wide_string(pstTemp);
    delete[] pstTemp;
    return pstReturn;
}
/*--------------------------------------------------------------------------*/
//windows : find main DLL and extract path
//linux and macos : scilab script fill SCI env variable
#ifdef _MSC_VER
char* computeSCI()
{
    char ScilabModuleName[MAX_PATH + 1];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    char *SciPathName = NULL;
    char *DirTmp = NULL;

    if (!GetModuleFileNameA((HINSTANCE)GetModuleHandleA("libScilab"), ScilabModuleName, MAX_PATH))
    {
        return NULL;
    }

    _splitpath(ScilabModuleName, drive, dir, fname, ext);

    if (dir[strlen(dir) - 1] == '\\')
    {
        dir[strlen(dir) - 1] = '\0';
    }

    DirTmp = strrchr(dir, '\\');
    if (strlen(dir) - strlen(DirTmp) > 0)
    {
        dir[strlen(dir) - strlen(DirTmp)] = '\0';
    }
    else
    {
        return NULL;
    }

    SciPathName = new char[strlen(drive) + strlen(dir) + 5];
    if (SciPathName)
    {
        _makepath(SciPathName, drive, dir, NULL, NULL);

        for (int i = 0 ; i < static_cast<int>(strlen(SciPathName)) ; i++)
        {
            if (SciPathName[i] == '\\')
            {
                SciPathName[i] = '/';
            }
        }
        SciPathName[strlen(SciPathName) - 1] = '\0';
    }
    return SciPathName;
}
#else
char* computeSCI()
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *SciPathName = new char[PATH_MAX];

    if (SciPathName)
    {
        getenvc(&ierr, "SCI", SciPathName, &lbuf, &iflag);

        if (ierr == 1)
        {
            cerr << "SCI environment variable not defined." << endl;
            exit(1);
        }
    }

    return SciPathName;
}
#endif
/*--------------------------------------------------------------------------*/
void defineSCI()
{
    wchar_t* sci_path = computeSCIW();
    setSCIW(sci_path);
    putenvSCIW(sci_path);
    FREE(sci_path);
}

