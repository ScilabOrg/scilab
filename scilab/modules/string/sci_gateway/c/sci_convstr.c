/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/* desc : converts the matrix of strings  str-matrix  into lower case (for*/
/*          "l"  ;default value) or upper case (for  "u" ).               */
/*        [y]=convstr(str-matrix, ["flag"])                               */
/*              str-matrix, y : matrices of strings                       */
/*              "flag"  : string ( "u"  for upper or  "l"  for lower      */
/*                        (default value))                                */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "wchar.h"
#include "convstr.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
int sci_convstr(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    wchar_t **Input_Matrix = NULL;
    wchar_t **Output_Matrix = NULL;
    char typ = LOW;
    int numRow = 0;
    int numCol = 0;
    int mn = 0; /* numRow * numCol */
    int i = 0;

    int* pNumOne = NULL;
    int Type_One = 0;

    int* pNumTwo = NULL;
    int Type_Two = 0;

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &pNumOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, pNumOne, &Type_One);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    if (Rhs == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &pNumTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, pNumTwo, &Type_Two);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (Type_Two == sci_strings)
        {
            int Row_Num_Two = 1, Col_Num_Two = 1;

            sciErr = getVarDimension(pvApiCtx, pNumTwo, &Row_Num_Two, &Col_Num_Two);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            if ( (Row_Num_Two * Col_Num_Two) == 1 )
            {
                int iLen = 1;
                char* pTyp = &typ;

                /* To put "flag" into typ; whether "u" or "l" */
                sciErr = getMatrixOfString(pvApiCtx, pNumTwo, &Row_Num_Two, &Col_Num_Two, &iLen, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                if (iLen != 1)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: 'u' (Upper) or 'l' (Lower) expected.\n"), fname, 2);
                    return 0;
                }

                sciErr = getMatrixOfString(pvApiCtx, pNumTwo, &Row_Num_Two, &Col_Num_Two, &iLen, &pTyp);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                if ( (typ != UPPER) && (typ != LOW) && (typ != UPPER_B) && (typ != LOW_B) )
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: 'u' (Upper) or 'l' (Lower) expected.\n"), fname, 2);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }
    }

    switch (Type_One)
    {
        case sci_strings :
        {
            int err;

            err = getAllocatedMatrixOfWideString(pvApiCtx, pNumOne, &numRow, &numCol, &Input_Matrix); /* To input the string matrix */
            if (err != 0)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
                return 0;
            }

            mn = numRow * numCol;
        }
        break;
        case sci_matrix :
        {
            /* Empty matrix case */
            if (isEmptyMatrix(pvApiCtx, pNumOne))
            {
                int err;

                err = createEmptyMatrix(pvApiCtx, Rhs + 1);
                if (err)
                {
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }

                LhsVar(1) = Rhs + 1 ;
                PutLhsVar();
                return 0;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
                return 0;
            }
        }
        break;
        default :
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
            return 0;
            break;
    }

    Output_Matrix = (wchar_t**)MALLOC(sizeof(wchar_t*) * (mn));
    if (Output_Matrix == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < mn; i++)
    {
        Output_Matrix[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(Input_Matrix[i]) + 1));
        if (Output_Matrix[i] == NULL)
        {
            freeArrayOfWideString(Output_Matrix, i);
            Scierror(999, ("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    /* convstr algorithm */
    convstr(Input_Matrix, Output_Matrix, typ, mn);
    freeArrayOfWideString(Input_Matrix, mn);

    /* put on scilab stack */
    sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, numRow, numCol, Output_Matrix);
    freeArrayOfWideString(Output_Matrix, mn);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1 ;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
