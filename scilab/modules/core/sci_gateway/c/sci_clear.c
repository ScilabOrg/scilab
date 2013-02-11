/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "call_scilab.h"
#include "isScilabFunction.h"
/*--------------------------------------------------------------------------*/
#undef istk
#define istk ((int *)&C2F(stack))
/*--------------------------------------------------------------------------*/
int clearNamedVariableOnStack(const char *varname);
int clearAllVariablesOnStack(void);
static int checkNamedVariableFormat(const char *_pstName);
/*--------------------------------------------------------------------------*/
static int sci_clear_no_rhs(const char *fname);
static int sci_clear_n_rhs(const char *fname);
/*--------------------------------------------------------------------------*/
extern int C2F(stackp)(int *, int *); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
int C2F(sci_clear)(char *fname,unsigned long fname_len)
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
        if (C2F(iop).lin[k + 6] != 0 && ((int *)&C2F(stack))[C2F(iop).lin[k + 5] - 1] == 10)
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
    int i = 0;
    int nbVariables = Rhs;
    char **VariableNameToClear = (char **)MALLOC(sizeof(char*) * nbVariables);

    if (!VariableNameToClear)
    {
        Scierror(999,_("%s: No more memory.\n"), fname);
        return 0;
    }

    for (k = 0; k < nbVariables; k++)
    {
        int *piAddressVar = NULL;
        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, k + 1, &piAddressVar);
        if (!sciErr.iErr)
        {
            if (isScalar(pvApiCtx, piAddressVar) && isStringType(pvApiCtx, piAddressVar))
            {
                char *variablename = NULL;
                if (getAllocatedSingleString(pvApiCtx, piAddressVar, &variablename) == 0)
                {
                    if (checkNamedVariableFormat(variablename) == 0)
                    {
                        freeAllocatedSingleString(variablename);
                        variablename = NULL;
                        freeAllocatedMatrixOfString(i, 1, VariableNameToClear);
                        VariableNameToClear = NULL;

                        Scierror(201, _("%s: Wrong value for argument %d: Valid variable name expected.\n"), fname, k + 1);
                        return 0;
                    }
                    else
                    {
                        VariableNameToClear[i] =  variablename;
                        i++;
                    }
                }
                else
                {
                    freeAllocatedMatrixOfString(i, 1, VariableNameToClear);
                    VariableNameToClear = NULL;
                    Scierror(999,_("%s: No more memory.\n"), fname);
                    return 0;
                }
            }
            else
            {
                freeAllocatedMatrixOfString(i, 1, VariableNameToClear);
                VariableNameToClear = NULL;

                if (isScalar(pvApiCtx, piAddressVar))
                {
                    Scierror(201, _("%s: Wrong type for argument %d: Valid variable name expected.\n"), fname, k + 1);
                    return 0;
                }
                else
                {
                    Scierror(999, _("%s: Wrong size for argument %d: Valid variable name expected.\n"), fname, k + 1);
                    return 0;
                }
            }
        }
    }

    for (k = 0; k < nbVariables; k++)
    {
        clearNamedVariableOnStack(VariableNameToClear[k]);
    }

    if (VariableNameToClear)
    {
        freeAllocatedMatrixOfString(nbVariables, 1, VariableNameToClear);
        VariableNameToClear = NULL;
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
    ++Top;
    il = iadr(C2F(vstk).lstk[Top - 1]);
    istk[il - 1] = 0;
    C2F(vstk).lstk[Top] = C2F(vstk).lstk[Top - 1] + 1;
    return 0;
}
/*--------------------------------------------------------------------------*/
int clearNamedVariableOnStack(const char *varname)
{
    int id[nsiz];
    int zero = 0;
    int il = 0;
    unsigned long name_len = (unsigned long)strlen(varname);

    C2F(str2name)(varname, id, name_len);
    il = iadr(C2F(vstk).lstk[Top - 1]);
    istk[il - 1] = 0;
    C2F(vstk).lstk[Top] = C2F(vstk).lstk[Top - 1] + 1;
    Rhs = 0;
    C2F(stackp)(id, &zero);
    if (Err > 0 || C2F(errgst).err1 > 0)
    {
        return 0;
    }
    Fin = 1;
    return 1;
}
/*--------------------------------------------------------------------------*/
static int checkNamedVariableFormat(const char *_pstName)
{
    #if _MSC_VER
    #define isascii __isascii
    #endif

    #define FORBIDDEN_CHARS " */\\.,;:^@><!=+-&|()~\n\t'\""
    int iRet = 1;
    int i = 0;

    // check pointer
    if (_pstName == NULL) iRet = 0;

    // check length _pstName =< nlgh
    if ((strlen(_pstName) == 0 || strlen(_pstName) > nlgh)) iRet = 0;

    // forbidden characters
    if (strpbrk(_pstName, FORBIDDEN_CHARS) != NULL) iRet = 0;

    // variable does not begin by a digit
    if (isdigit(_pstName[0])) iRet = 0;

    // check that we have only ascii characters
    for (i = 0; i < (int)strlen(_pstName); i++)
    {
        if (!isascii(_pstName[i])) iRet = 0;
        break;
    }

    // add here some others rules

    return iRet;
}
/*--------------------------------------------------------------------------*/
