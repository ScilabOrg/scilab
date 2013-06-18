/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "BOOL.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "call_scilab.h"
#include "isScilabFunction.h"
#include "api_scilab.h"

/*--------------------------------------------------------------------------*/
static int clearAllVariablesOnStack(void);
/*--------------------------------------------------------------------------*/
static int sci_clear_no_rhs(const char *fname);
static int sci_clear_n_rhs(const char *fname);
/*--------------------------------------------------------------------------*/
extern int C2F(stackp)(int *, int *); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
int C2F(sci_clear)(char *fname, unsigned long fname_len)
{
    if (Rhs == 0)
    {
        sci_clear_no_rhs(fname);
    }
    else
    {
        sci_clear_n_rhs(fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_clear_no_rhs(const char *fname)
{
    if (C2F(recu).macr != 0 || C2F(recu).paus != 0)
    {
        int k = 0;
        if (C2F(recu).rstk[C2F(recu).pt - 3] == 909)
        {
            /* clear within an exec'd macro */
            return clearAllVariablesOnStack();
        }
        /* clear within a macro, an execstr, an exec or a pause */
        k = C2F(iop).lpt[0] - 19;
        if (C2F(iop).lin[k + 6] != 0 && *istk(C2F(iop).lin[k + 5]) == 10)
        {
            PutLhsVar();
            return 0;
        }
        /* clear within a macro, an exec or a pause */
        /* Computing MIN */
        Bot = Min(C2F(vstk).bbot, C2F(iop).lin[k + 4]);
        PutLhsVar();
        return 0;
    }
    return clearAllVariablesOnStack();
}
/*--------------------------------------------------------------------------*/
static int sci_clear_n_rhs(const char *fname)
{
    int k = 0;
    int i = 0, j = 0;
    int nbVariables = Rhs;
    int nRows = 0, nCols = 0;
    char ** variablename = NULL;
    int * piAddressVar = NULL;

    for (k = 0; k < nbVariables; ++k)
    {
        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, k + 1, &piAddressVar);
        if (!sciErr.iErr)
        {
            if (isStringType(pvApiCtx, piAddressVar))
            {
                if (getAllocatedMatrixOfString(pvApiCtx, piAddressVar, &nRows, &nCols, &variablename) == 0)
                {
                    for (i = 0; i < nCols; ++i)
                        for (j = 0; j < nRows; ++j)
                        {
                            deleteNamedVariable(pvApiCtx, variablename[j + i * nRows]);
                        }
                    freeAllocatedMatrixOfString(nRows, nCols, variablename);
                }
                else
                {
                    SciError(17);
                    return 0;
                }
            }
            else
            {
                Scierror(207, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, k + 1);
                return 0;
            }
        }
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
int clearAllVariablesOnStack(void)
{
    /* clear all variables */
    int il = 0;
    Fin = 0;
    C2F(adre).is = Fin;
    Bot = C2F(vstk).bbot;

    //create a null matrix a the Top of the stack
    Top = Top + 1;
    il = iadr(*Lstk(Top));
    *istk(il) = 0;
    *Lstk(Top + 1) = *Lstk(Top) + 1;
    return 0;
}
