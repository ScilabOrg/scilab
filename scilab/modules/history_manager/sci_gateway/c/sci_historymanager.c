/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "gw_history_manager.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "localization.h"
#include "InitializeHistoryManager.h"
#include "TerminateHistoryManager.h"
#include "getCommentDateSession.h"
#include "Scierror.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_historymanager(char *fname, unsigned long fname_len)
{
    int l1 = 0, n1 = 0, m1 = 0;
    char *Output = NULL;

    CheckRhs(0, 1) ;
    CheckLhs(0, 1) ;

    if (Rhs == 0)
    {
        if (historyIsEnabled())
        {
            Output = strdup("on");
        }
        else
        {
            Output = strdup("off");
        }
    }
    else
    {
        if (GetType(1) == sci_strings)
        {
            char *param = NULL;

            GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
            param = cstk(l1);

            if ( (strcmp(param, "off") == 0) || (strcmp(param, "on") == 0) )
            {
                if (strcmp(param, "off") == 0)
                {
                    if (historyIsEnabled())
                    {
                        TerminateHistoryManager();
                    }
                    Output = strdup("off");
                }
                else /* 'on' */
                {

                    if (!historyIsEnabled())
                    {
                        char *commentbeginsession = NULL;
                        InitializeHistoryManager();

                        /* add date & time @ begin session */
                        commentbeginsession = getCommentDateSession(FALSE);
                        if (commentbeginsession)
                        {
                            appendLineToScilabHistory(commentbeginsession);
                            FREE(commentbeginsession);
                            commentbeginsession = NULL;
                        }
                    }
                    Output = strdup("on");
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be '%s' or '%s'.\n"), fname, 1, "on", "off");
                return 0;
            }
        }
    }

    n1 = 1;
    CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, (m1 = (int)strlen(Output), &m1), &n1, &Output);
    if (Output)
    {
        FREE(Output);
        Output = NULL;
    }
    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
