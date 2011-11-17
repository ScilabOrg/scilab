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
extern int C2F(intclear)(char *fname,unsigned long fname_len);
static int clearAllVariablesOnStack(void);
static int clearNamedVariableOnStack(const char *varname);
static int checkNamedVariableFormat(const char *_pstName);
static int callOverloadForMlist(int _iVar, const char *fname);
/*--------------------------------------------------------------------------*/
static int sci_clear_no_rhs(const char *fname);
static int sci_clear_one_rhs(const char *fname);
static int sci_clear_n_rhs(const char *fname);
/*--------------------------------------------------------------------------*/
extern int C2F(stackp)(int *, int *); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
int C2F(sci_clear)(char *fname,unsigned long fname_len)
{
    switch(Rhs)
    {
    case 0:
        return sci_clear_no_rhs(fname);
    case 1:
        return sci_clear_one_rhs(fname);
    default:
        return sci_clear_n_rhs(fname);
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
static int sci_clear_one_rhs(const char *fname)
{
    int iTypeOne = 0;
    int *piAddressVarOne = NULL;
    SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iTypeOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iTypeOne == sci_mlist) 
    {
        return callOverloadForMlist(1, fname);
    }
    return sci_clear_n_rhs(fname);
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
static int clearAllVariablesOnStack(void)
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
static int clearNamedVariableOnStack(const char *varname)
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
static int callOverloadForMlist(int _iVar, const char *fname)
{
    int nbItem = 0;
    int* piAddrChild = NULL;
    int *piAddressVar = NULL;

    SciErr sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if (sciErr.iErr != 0) return 0;
    sciErr = getListItemNumber(pvApiCtx, piAddressVar, &nbItem);
    if (sciErr.iErr != 0) return 0;
    if (nbItem < 1) return 0;
    sciErr = getListItemAddress(pvApiCtx, piAddressVar, 1, &piAddrChild);
    if (sciErr.iErr != 0) return 0;
    if (!isStringType(pvApiCtx, piAddrChild)) 
    {
        return 0;
    }
    else
    {
        int m = 0, n = 0;
        char **pstrData = NULL;
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrChild, &m, &n, &pstrData) != 0)
        {
            return 0;
        }
        else
        {
            char overloadFunctionName[nlgh];
            int lengthOverloadFunctionName = (int)(strlen("%") + strlen(pstrData[0]) + strlen("_") + strlen(fname) + 1);            
            if (lengthOverloadFunctionName <= nlgh) // length of function name in S5
            {
                strcpy(overloadFunctionName, "%");
                strcat(overloadFunctionName, pstrData[0]);
                freeAllocatedMatrixOfString(m, n, pstrData);
                pstrData = NULL;

                strcat(overloadFunctionName, "_");
                strcat(overloadFunctionName, fname);

                if (isScilabFunction(overloadFunctionName))
                {
                    char varname[nlgh];
                    int lw = 1 + Top - Rhs;
                    int numberOfLhsOnScilabFunction = 1;
                    int numberOfRhsOnScilabFunction = 1;
                    Nbvars = 1;
                    C2F(scistring)(&lw, overloadFunctionName, &numberOfLhsOnScilabFunction, 
                        &numberOfRhsOnScilabFunction,
                        (unsigned long) strlen(overloadFunctionName) );
                    sciErr = getVarNameFromPosition(pvApiCtx, _iVar, varname);
                    if (sciErr.iErr == 0)
                    {
                        clearNamedVariableOnStack(varname);
                        PutLhsVar();
                        return 0;
                    }
                }
            }
        }
    }
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
