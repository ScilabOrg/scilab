/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "gw_history_manager.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "MALLOC.h"
#include "expandPathVariable.h"
/*--------------------------------------------------------------------------*/
int sci_savehistory(char *fname, unsigned long fname_len)
{
    CheckRhs(0, 1) ;
    CheckLhs(0, 1) ;

    if (Rhs == 0)
    {
        char *filename = getFilenameScilabHistory();
        if (filename)
        {
            writeScilabHistoryToFile(filename);
            FREE(filename);
            filename = NULL;
        }
    }
    else
    {
        char  *line = NULL;
        char *Path = NULL;
        int l1 = 0, m1 = 0, n1 = 0;

        if ( GetType(1) == sci_strings )
        {
            GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
            Path = cstk(l1);

            line = expandPathVariable(Path);
            if (line)
            {
                writeScilabHistoryToFile(line);
                FREE(line);
                line = NULL;
            }
        }
    }
    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
