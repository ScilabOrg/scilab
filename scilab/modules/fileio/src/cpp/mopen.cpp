/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "file.hxx"
#include "filemanager.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "mopen.h"
#include "sciprint.h"
#include "charEncoding.h"
#include "warningmode.h"
#include "isdir.h"
#include "localization.h"
#include "os_wfopen.h"
}
/*--------------------------------------------------------------------------*/
int mopen(const wchar_t* _pstFilename, const wchar_t* _pstMode, int _iSwap, int* _piID)
{
    int lenChar = 0, testRep = 1;

    if (getWarningMode() && FileManager::isOpened(_pstFilename))
    {
        char* pst = wide_string_to_UTF8(_pstFilename);
        sciprint(_("Warning: file '%s' already opened in Scilab.\n"), pst);
        FREE(pst);
    }
    /* bug 4846 */
    if (_pstFilename == NULL)
    {
        return MOPEN_INVALID_FILENAME;
    }

    if (wcslen(_pstFilename) == 0)
    {
        return MOPEN_INVALID_FILENAME;
    }

    if (_pstMode == NULL)
    {
        return MOPEN_INVALID_STATUS;
    }

    lenChar = wcslen(_pstMode);
    if (( lenChar == 0 ) || ( lenChar > 3 ))
    {
        return MOPEN_INVALID_STATUS;
    }

    if ((_pstMode[0] != L'a') && (_pstMode[0] != L'r') && (_pstMode[0] != L'w'))
    {
        return MOPEN_INVALID_STATUS;
    }

    for ( testRep = 1; testRep < lenChar ; testRep++ )
    {
        if (( _pstMode[testRep] != '+' ) && ( _pstMode[testRep] != 'b' ) && ( _pstMode[testRep] != 't' ))
        {
            return MOPEN_INVALID_STATUS;
        }
    }

    if (isdirW(_pstFilename))
    {
        return MOPEN_CAN_NOT_OPEN_FILE;
    }

    FILE* pF = os_wfopen(_pstFilename, _pstMode);
    if (pF == NULL)
    {
        return MOPEN_CAN_NOT_OPEN_FILE;
    }

    //Create File object and fill it
    types::File* pFile = new types::File();
    pFile->setFileDesc(pF);
    pFile->setFilename(_pstFilename);
    pFile->setFileType(2); //hard coded value for file opened by C/C++ fopen functions
    pFile->setFileMode(_pstMode);
    pFile->setFileSwap(_iSwap);

    *_piID = FileManager::addFile(pFile);
    return MOPEN_NO_ERROR;
}
/*--------------------------------------------------------------------------*/

