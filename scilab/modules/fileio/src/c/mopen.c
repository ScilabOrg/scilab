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
#include <stdio.h>
#include <string.h>
#include "mopen.h"
#include "Status2Mode.h"
#include "filesmanagement.h"
#include "getfiledesc.h"
#include "addfile.h"
#include "sciprint.h"
#include "localization.h"
#include "warningmode.h"
#include "MALLOC.h"
#include "BOOL.h"
#include "charEncoding.h"
#include "isdir.h"
/*--------------------------------------------------------------------------*/
extern int swap;
/*--------------------------------------------------------------------------*/
void C2F(mopen)(int *fd, char *file, char *status, int *f_swap, double *res, int *error)
{
    int littlendian = 1, type = 2, ierr, mode;
    int lenChar = 0, testRep = 1;
    char *endptr;
    FILE * fa;

    /* bug 4846 */
    if (file == NULL)
    {
        *error = (int)MOPEN_INVALID_FILENAME;
        return;
    }

    if ( getWarningMode() && IsAlreadyOpenedInScilab(file) )
    {
        sciprint(_("Warning: file '%s' already opened in Scilab.\n"), file);
    }

    swap = 0;
    *error = (int)MOPEN_NO_ERROR;
    endptr = (char *) &littlendian;
    if ( (!*endptr) )
    {
        if ( *f_swap == 1 )
        {
            swap = 1;
        }
        else
        {
            swap = 0;
        }
    }

    C2F(getfiledesc)(fd);

    if ( *fd == -1 )
    {
        *error = (int)MOPEN_NO_MORE_LOGICAL_UNIT;
        return;
    }

    if (strlen(file) == 0)
    {
        *error = (int)MOPEN_INVALID_FILENAME;
        return;
    }

    if (status == NULL)
    {
        *error = (int)MOPEN_INVALID_STATUS;
        return;
    }

    //#Bug 13331 //
    lenChar = (int)strlen(status);
    if (( lenChar == 0 ) || ( lenChar > 3 ))
    {
        *error = (int)MOPEN_INVALID_STATUS;
        return;
    }

    if ((status[0] != 'a') && (status[0] != 'r') && (status[0] != 'w'))
    {
        *error = (int)MOPEN_INVALID_STATUS;
        return;
    }

    for ( testRep = 1; testRep < lenChar ; testRep++ )
    {
        if (( status[testRep] != '+' ) && ( status[testRep] != 'b' ) && ( status[testRep] != 't' ))
        {
            *error = (int)MOPEN_INVALID_STATUS;
            return;
        }
    }

    if (isdir(file))
    {
        *error = (int)MOPEN_CAN_NOT_OPEN_FILE;
        return;
    }

    wcfopen(fa , file, status);

    if (! fa )
    {
        *error = (int)MOPEN_CAN_NOT_OPEN_FILE;
        return;
    }
    mode = Status2Mode(status);
    C2F(addfile)(fd, fa, &swap, &type, &mode, file, &ierr);
    if (ierr)
    {
        *error = (int)MOPEN_NO_MORE_MEMORY;
        return;
    }
    SetCurrentFileId(*fd);
    *res = (double)ferror(fa);
}
/*--------------------------------------------------------------------------*/

