/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/* res = strspn(str1,str2) */
/* Returns the length of the initial portion of str1 */
/* which consists only of characters that are part of str2. */
/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
/*----------------------------------------------------------------------------*/
int sci_strspn(char *fname, unsigned long fname_len)
{
    CheckRhs(2, 2);
    CheckLhs(0, 1);

    if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
    {
        int m1 = 0;
        int n1 = 0;
        char **InputString_Parameter1 = NULL;
        int m1n1 = 0; /* m1 * n1 */

        int m2 = 0;
        int n2 = 0;
        char **InputString_Parameter2 = NULL;
        int m2n2 = 0; /* m2 * n2 */

        GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, &InputString_Parameter1);
        m1n1 = m1 * n1;

        GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &m2, &n2, &InputString_Parameter2);
        m2n2 = m2 * n2;

        if ( ((m2 == m1) && (n2 == n1)) || (m2n2 == 1) )
        {
            int i = 0;
            int j = 0;
            int outIndex = 0;

            /*Output*/
            CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &outIndex);
            for ( i = 0 ; i < m1n1 ; i++ )
            {
                wchar_t *wcParam1 = NULL;
                wchar_t *wcParam2 = NULL;

                if (m2n2 == 1)
                {
                    j = 0;
                }
                else
                {
                    j = i;
                }

                wcParam1 = to_wide_string(InputString_Parameter1[i]);
                wcParam2 = to_wide_string(InputString_Parameter2[j]);

                if (wcParam1 && wcParam2)
                {
                    stk(outIndex)[i] = (double) wcsspn( wcParam1, wcParam2 );
                }
                else
                {
                    stk(outIndex)[i] = (double) strspn( InputString_Parameter1[i], InputString_Parameter2[j] );
                }

                if (wcParam1)
                {
                    FREE(wcParam1);
                    wcParam1 = NULL;
                }
                if (wcParam2)
                {
                    FREE(wcParam2);
                    wcParam2 = NULL;
                }
            }
            LhsVar(1) = Rhs + 1 ;
            PutLhsVar();
        }
        else
        {
            freeArrayOfString(InputString_Parameter1, m1n1);
            freeArrayOfString(InputString_Parameter2, m2n2);
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 2);
            return 0;
        }

        freeArrayOfString(InputString_Parameter1, m1n1);
        freeArrayOfString(InputString_Parameter2, m2n2);
    }
    else
    {
        if (GetType(1) != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 2);
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
