
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
#include "MALLOC.h"
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

int * stringsCompare(char **Input_String_One, int dim_One, char **Input_String_Two, int dim_Two, BOOL dostricmp)
{
    int *returnedValues = NULL;
    int i_One = 0;
    int i_Two = 0;
    int dim_Max = (dim_One >= dim_Two) ? dim_One : dim_Two;

    returnedValues = (int*)MALLOC(sizeof(int) * dim_Max);
    if (returnedValues)
    {
        int i = 0;
        for (i = 0; i < dim_Max; i++)
        {
            i_One = (dim_One == 1) ? 0 : i;
            i_Two = (dim_Two == 1) ? 0 : i;

            if (dostricmp)
            {
                returnedValues[i] = sign(stricmp(Input_String_One[i_One], Input_String_Two[i_Two]));
            }
            else
            {
                returnedValues[i] = sign(strcmp(Input_String_One[i_One], Input_String_Two[i_Two]));
            }
        }
    }
    return returnedValues;
}
/*--------------------------------------------------------------------------*/
