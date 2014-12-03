/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2012 - Allan CORNET
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "BOOL.h"
#include "vect_or.h"
/*--------------------------------------------------------------------------*/
void vect_or(const int *v, int *dimsArray, int dims, int *r, int opt)
{
    int siz = 1;
    int i = 0;
    int sizeD = 1;
    int k = 0;
    int l = 0;

    for (i = 0; i < dims; i++)
    {
        siz *= dimsArray[i];
    }
    if (opt == 1)
    {
        for (k = 0; k < siz / dimsArray[opt - 1]; k++)
        {
            i = k * dimsArray[0];
            r[k] = FALSE;
            for (l = 0; l < dimsArray[0]; l++)
            {
                if (v[i++])
                {
                    r[k] = TRUE;
                    break;
                }
            }
        }
    }
    else if (opt == 2)
    {
        for (l = 0; l < siz / dimsArray[opt - 1]; l++)
        {
            i = l * dimsArray[1];
            if (i % 2 == 1)
            {
                i -= 2;
            }
            r[l] = FALSE;
            for (k = 0; k < dimsArray[1]; k++)
            {
                if (v[i])
                {
                    r[l] = TRUE;
                    break;
                }
                i += dimsArray[0];
            }
        }
    }
    else
    {
        for (i = 0; i < opt - 1; i++)
        {
            sizeD *= dimsArray[i];
        }


        r[0] = FALSE;

        for (k = 0; k < (siz / dimsArray[opt - 1]); k++)
        {
            i = k;
            r[k] = FALSE;
            for (l = 0; l < dimsArray[opt - 1]; l++)
            {
                if (v[i])
                {
                    r[k] = TRUE;
                    break;
                }
                i += sizeD;
            }
        }
    }

}

/*--------------------------------------------------------------------------*/
