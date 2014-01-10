
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
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
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include "gw_string.h"
#include "convstr.h"
/*--------------------------------------------------------------------------*/
void convstr(wchar_t **Input_Matrix, wchar_t **Output_Matrix, char typ, int mn)
{
    int x = 0;
    for (x = 0 ; x < mn; x++)
    {
        int y = 0;
        size_t len_Input_Matrix = wcslen(Input_Matrix[x]);
        for (y = 0; y < len_Input_Matrix; y++)
        {
            /*To traverse every string in the string matrix */
            if ( (typ == UPPER) || (typ == UPPER_B) )
            {
                /*converts the matrix of strings  str-matrix into upper case */
                Output_Matrix[x][y] = (wchar_t) towupper(Input_Matrix[x][y]);
            }
            else if ( (typ == LOW) || (typ == LOW_B) )
            {
                /*converts the matrix of strings  str-matrix  into lower case */
                Output_Matrix[x][y] = (wchar_t) towlower(Input_Matrix[x][y]);
            }
        }
        Output_Matrix[x][y] = 0;
    }
}
/*--------------------------------------------------------------------------*/
