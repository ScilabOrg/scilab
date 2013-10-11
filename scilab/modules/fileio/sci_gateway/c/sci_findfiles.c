/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_fileio.h"
#include "findfiles.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "scicurdir.h"          /* scigetcwd */
#include "Scierror.h"
#include "expandPathVariable.h"
#include "PATH_MAX.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define DEFAULT_FILESPEC "*.*"
#else
#define DEFAULT_FILESPEC "*"
#endif
/*--------------------------------------------------------------------------*/
int sci_findfiles(char *fname, unsigned long fname_len)
{
    static int l1 = 0, n1 = 0, m1 = 0;
    char *pathextented = NULL;
    char *path = NULL;
    char *filespec = NULL;
    char **FilesList = NULL;
    int sizeListReturned = 0;

    Rhs = Max(Rhs, 0);
    CheckRhs(0, 2);
    CheckLhs(0, 1);

    switch (Rhs)
    {
        default:
        case 0:
        {
            int ierr = 0;

            path = scigetcwd(&ierr);

            if (ierr)
            {
                Scierror(999, _("%s: Error while trying to retrieve the name of the current directory.\n"), fname);
                return 0;
            }
            else
            {
                filespec = strdup(DEFAULT_FILESPEC);
            }
        }
        break;

        case 1:
        {
            if (GetType(1) == sci_strings)
            {
                GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
                path = strdup(cstk(l1));
                filespec = strdup(DEFAULT_FILESPEC);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
                return 0;
            }

        }
        break;

        case 2:
        {
            if ((GetType(1) == sci_strings) && (GetType(2) == sci_strings))
            {
                GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
                path = strdup(cstk(l1));

                GetRhsVar(2, STRING_DATATYPE, &m1, &n1, &l1);
                filespec = strdup(cstk(l1));
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input arguments: Strings expected.\n"), fname);
                return 0;
            }
        }
        break;
    }

    pathextented = expandPathVariable(path);
    FilesList = findfiles(pathextented, filespec, &sizeListReturned, FALSE);

    FREE(path);
    FREE(pathextented);
    FREE(filespec);

    if (FilesList)
    {
        int ncol = 0, nrow = 0;

        ncol = 1;
        nrow = sizeListReturned;

        CreateVarFromPtr(Rhs + 1, MATRIX_OF_STRING_DATATYPE, &nrow, &ncol, FilesList);
        LhsVar(1) = Rhs + 1;
    }
    else
    {
        n1 = 0;
        m1 = 0;
        l1 = 0;
        CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &n1, &m1, &l1);
        LhsVar(1) = Rhs + 1;
    }

    freeArrayOfString(FilesList, sizeListReturned);

    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
