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
    int siz = 1, i = 0;

    for (i = 0; i < dims; i++)
    {
        siz *= dimsArray[i];
    }
    int k = 0;
    r[0] = FALSE;

    for (k = 0; k < siz / dimsArray[opt - 1]; k++)
    {
        int l = 0;
        int i = k * dimsArray[opt - 1];
        r[k] = FALSE;
        for (l = 0; l < dimsArray[opt - 1]; l++)
        {
            if (v[i++])
            {
                r[k] = TRUE;
                break;
            }
        }
    }

}

/*--------------------------------------------------------------------------*/
