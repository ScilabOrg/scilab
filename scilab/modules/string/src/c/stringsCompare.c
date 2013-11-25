
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

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "stringsCompare.h"
#include "SCIMALLOC.h"
#include "stricmp.h"
/*--------------------------------------------------------------------------*/
static int sign(int x)
{
    if (x < 0)
    {
        return -1;
    }
    if (x > 0)
    {
        return 1;
    }
    return 0;
}

int * stringsCompare(wchar_t **Input_String_One, int dim_One, wchar_t **Input_String_Two, int dim_Two, BOOL dostricmp)
{
    int *returnedValues = NULL;
    int j = 0;

    returnedValues = (int*)MALLOC(sizeof(int) * dim_One);
    if (returnedValues)
    {
        int i = 0;
        for (i = 0; i < dim_One; i++)
        {
            if (dim_Two == 1)
            {
                j = 0;
            }
            else
            {
                j = i;
            }

            if (dostricmp)
            {
                returnedValues[i] = sign(wcsicmp(Input_String_One[i], Input_String_Two[j]));
            }
            else
            {
                returnedValues[i] = sign(wcscmp(Input_String_One[i], Input_String_Two[j]));
            }
        }
    }
    return returnedValues;
}
/*--------------------------------------------------------------------------*/
