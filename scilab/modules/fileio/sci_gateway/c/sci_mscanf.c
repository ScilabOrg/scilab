/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include "Scierror.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "do_xxscanf.h"
#include "do_xxprintf.h"
#include "gw_fileio.h"
#include "localization.h"
#include "scanf_functions.h"
#include "StringConvert.h"
#include "xscion.h"
#include "../../../console/includes/GetCommandLine.h"   /* getConsoleInputLine */
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_scanf(char *fname, unsigned long fname_len)
{
    static char *String = NULL;
    static int l1 = 0, m1 = 0, n1 = 0, iarg = 0, maxrow = 0, nrow = 0, rowcount = 0, ncol = 0;
    int args = 0, retval = 0, retval_s = 0, lline = 0, iflag = 0, err = 0, n_count = 0;
    entry *data = NULL;
    rec_entry buf[MAXSCAN];
    sfdir type[MAXSCAN] = {SF_C};
    sfdir type_s[MAXSCAN] = {SF_C};

    Nbvars = 0;
    CheckRhs(1, 2);

    if (Rhs == 2)
    {
        GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
        if (m1 * n1 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Scalar expected.\n"), fname, 1);
            return 0;
        }
        iarg = 2;
        maxrow = *istk(l1);
    }
    else
    {
        iarg = 1;
        maxrow = 1;
    }

    GetRhsVar(iarg, STRING_DATATYPE, &m1, &n1, &l1);
    /** format **/
    n_count = StringConvert(cstk(l1)) + 1;  /* conversion */

    if (n_count > 1)
    {
        Scierror(999, _("%s: Specified format cannot include any '\\n'\n"), fname);
        return 0;
    }

    nrow = maxrow;
    rowcount = -1;              /* number-1 of result lines already got */
    while (1)
    {
        rowcount++;
        if ((maxrow >= 0) && (rowcount >= maxrow))
        {
            break;
        }

        /* get a line */
        C2F(xscion) (&iflag);

        String = getConsoleInputLine();
        if (String == NULL)
        {
            Scierror(999, _("%s: Data mismatch.\n"), fname);
            return 0;
        }
        lline = (int)strlen(String);

        if (lline == 0)
        {
            FREE(String);
            String = strdup(" ");
        }

        /** use the scaned line as input **/
        args = Rhs;             /* args set to Rhs on entry */
        if (do_xxscanf("scanf", (FILE *) 0, cstk(l1), &args, String, &retval, buf, type) < 0)
        {
            if (String)
            {
                FREE(String);
                String = NULL;
            }
            return 0;
        }
        if (String)
        {
            FREE(String);
            String = NULL;
        }

        if ((err = Store_Scan(&nrow, &ncol, type_s, type, &retval, &retval_s, buf, &data, rowcount, args)) < 0)
        {
            switch (err)
            {
                case MISMATCH:
                    if (maxrow >= 0)
                    {
                        Free_Scan(rowcount, ncol, type_s, &data);
                        Scierror(999, _("%s: Data mismatch.\n"), fname);
                        return 0;
                    }
                    break;

                case MEM_LACK:
                    Free_Scan(rowcount, ncol, type_s, &data);
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
            }
        }
    }                           /*  while (1) */

    /* create Scilab variables with each column of data */
    err = Sci_Store(rowcount, ncol, data, type_s, retval_s);
    Free_Scan(rowcount, ncol, type_s, &data);
    if (err == MEM_LACK)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
