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
#ifndef __ARCHIVE_LIST_H__
#define __ARCHIVE_LIST_H__

#include "dynlib_fileio.h"

/**
* Creates an absolute or full path name for the specified relative path name.
* @param[out] absPath Pointer to a buffer containing the absolute or full path name, or NULL.
* @param[in] relPath Relative path name.
* @param[in] maxLength Maximum length of the absolute path name buffer (absPath).
* @return returns a pointer to a buffer containing the absolute path name (absPath).
*/

FILEIO_IMPEXP char** archive_list(char *filename,int *size);


#endif
/*--------------------------------------------------------------------------*/
