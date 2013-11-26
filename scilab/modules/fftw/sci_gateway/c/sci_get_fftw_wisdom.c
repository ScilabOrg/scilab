/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Alan LAYEC
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "callfftw.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "gw_fftw.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
/* Return text of fftw wisdom
*
* Scilab Calling sequence :
*   -->tt=get_fftw_wisdom();
*
* Input : Nothing
*
* Output : a scilab string matrix
*
*/
int sci_get_fftw_wisdom(char *fname, unsigned long fname_len)
{
    int n1 = 0, i = 0, j = 0;
    char *Str = NULL;
    char **Str1 = NULL;

    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if ((Str = call_fftw_export_wisdom_to_string()) == NULL)
    {
        Scierror(999, _("%s: MKL fftw library does not implement wisdom functions yet.\n"), fname);
        return 1;
    }

    n1 = 0;
    j = 0;
    if (Str)
    {
        int lenStr = (int)strlen(Str);
        for (i = 0; i < lenStr; i++)
        {
            if (Str[i] == '\n')
            {
                int len = 0;
                int k = 0;

                n1++;

                if (Str1)
                {
                    Str1 = (char **)REALLOC(Str1, sizeof(char *) * n1);
                }
                else
                {
                    Str1 = (char **)MALLOC(sizeof(char *) * n1);
                }

                if (Str1 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    if (Str)
                    {
                        // According to the FFTW documentation we should free Str
                        // string but doing makes Scilab crash!?
                        //free(Str);
                    }
                    return 1;
                }

                len = i - j;

                if ((Str1[n1 - 1] = (char *)MALLOC(sizeof(char) * (len + 1))) == NULL)
                {
                    freeArrayOfString(Str1, n1 - 1);
                    if (Str)
                    {
                        // According to the FFTW documentation we should free Str
                        // string but doing makes Scilab crash!?
                        //free(Str);
                    }
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 1;
                }

                for (k = 0; k < len; k++)
                {
                    Str1[n1 - 1][k] = Str[k + j];
                }
                Str1[n1 - 1][len] = '\0';
                j = i + 1;
            }
        }
    }

    n1++;

    if (Str1)
    {
        Str1 = (char **)REALLOC(Str1, sizeof(char *) * n1);
    }
    else
    {
        Str1 = (char **)MALLOC(sizeof(char *) * n1);
    }

    if (Str1 == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        if (Str)
        {
            // According to the FFTW documentation we should free Str
            // string but doing makes Scilab crash!?
            //free(Str);
        }
        return 1;
    }

    if ((Str1[n1 - 1] = (char *)MALLOC(sizeof(char))) == NULL)
    {
        freeArrayOfString(Str1, n1 - 1);
        if (Str)
        {
            // According to the FFTW documentation we should free Str
            // string but doing makes Scilab crash!?
            //free(Str);
        }
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 1;
    }
    Str1[n1 - 1][0] = '\0';

    createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, n1, 1, Str1);

    freeArrayOfString(Str1, n1);
    if (Str)
    {
        // According to the FFTW documentation we should free Str
        // string but doing makes Scilab crash!?
        //free(Str);
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
