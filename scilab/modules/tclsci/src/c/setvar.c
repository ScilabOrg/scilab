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

#include "setvar.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
BOOL SetVarMatrix(Tcl_Interp *TCLinterpreter, char *VarName, int ptrValues, int m, int n)
{
    BOOL bOK = TRUE;
    int i = 0, j = 0;

    double *MatrixDouble = (double*)MALLOC((m * n) * sizeof(double));

    /* Efface valeur precedente */
    Tcl_UnsetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);

    for (i = 0; i < m * n; i++)
    {
        MatrixDouble[i] = *stk(ptrValues++);
    }


    for (i = 1; i < m + 1; i++)
    {
        char VarValueWithIndice[2048];
        char VarNameWithIndice[2048];

        for (j = 1; j < n + 1; j++)
        {
            int bOKsprintf1 = 0;
            int bOKsprintf2 = 0;

            bOKsprintf1 = sprintf(VarNameWithIndice, "%s(%d,%d)", VarName, i, j);
            bOKsprintf2 = sprintf(VarValueWithIndice, "%.10lf", MatrixDouble[(i - 1) + m * (j - 1)]);

            if ( (bOKsprintf1 == -1) || (bOKsprintf2 == -1) )
            {
                Scierror(999, _("Variable too long.\n"));
                return FALSE;
            }

            if (TCLinterpreter == NULL)
            {
                Scierror(999, _("%s: Error TCL interpreter not initialized.\n"), "TCL_SetVar");
                return FALSE;
            }

            if (!Tcl_SetVar(TCLinterpreter, VarNameWithIndice, VarValueWithIndice, 0))
            {
                bOK = FALSE ;
            }
        }
    }
    if (MatrixDouble)
    {
        FREE(MatrixDouble);
        MatrixDouble = NULL;
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL SetVarScalar(Tcl_Interp *TCLinterpreter, char *VarName, double VarValue)
{

    /* just a scalar */
    char buffer[2048];

    sprintf(buffer, "%.10lf", VarValue);

    if (TCLinterpreter == NULL)
    {
        Scierror(999, _("%s: Error TCL interpreter not initialized.\n"), "TCL_SetVar");
        return FALSE;
    }

    /* Efface valeur precedente */
    Tcl_UnsetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);

    if (!Tcl_SetVar(TCLinterpreter, VarName, buffer, TCL_GLOBAL_ONLY))
    {
        return FALSE ;
    }
    else
    {
        return TRUE;
    }
}
/*--------------------------------------------------------------------------*/
BOOL SetVarStrings(Tcl_Interp *TCLinterpreter, char *VarName, char **Str, int m, int n)
{

    int i = 0, j = 0;
    int l = 0;
    BOOL TestOnAllTcl_SetVar = TRUE;

    if (TCLinterpreter == NULL)
    {
        Scierror(999, _("%s: Error TCL interpreter not initialized.\n"), "TCL_SetVar");
        return FALSE;
    }

    for (j = 1; j < n + 1; j++)
    {
        for (i = 1; i < m + 1; i++)
        {
            char VarArrayName[1024];
            sprintf(VarArrayName, "%s(%d,%d)", VarName, i, j);
            if (Tcl_SetVar(TCLinterpreter, VarArrayName, Str[l++], TCL_GLOBAL_ONLY))
            {
                if (TestOnAllTcl_SetVar)
                {
                    TestOnAllTcl_SetVar = TRUE;
                }
            }
            else
            {
                TestOnAllTcl_SetVar = FALSE;
            }
        }
    }
    return TestOnAllTcl_SetVar;

}
/*--------------------------------------------------------------------------*/
BOOL SetVarAString(Tcl_Interp *TCLinterpreter, char *VarName, char **Str)
{

    if (TCLinterpreter == NULL)
    {
        Scierror(999, _("%s: Error TCL interpreter not initialized.\n"), "TCL_SetVar");
        return FALSE;
    }

    if ( !Tcl_SetVar(TCLinterpreter, VarName, Str[0], TCL_GLOBAL_ONLY) )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}
/*--------------------------------------------------------------------------*/
