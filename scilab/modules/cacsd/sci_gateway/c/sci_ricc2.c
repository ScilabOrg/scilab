/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "sci_rankqr.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

extern int C2F(riccsl)();
extern int C2F(riccms)();
extern int C2F(ricdsl)();
extern int C2F(ricdmf)();

int sci_ricc2(char *fname, unsigned long fname_len)
{
    BOOL WANTC = 0, WANTD = 0, WSCHUR = 0, WSIGN = 0, WINVF = 0;

    int minrhs = 4;
    int maxrhs = 5;
    int minLhs = 1;
    int maxLhs = 3;

    int N = 0, LWORKMIN = 0, INFO = 0;
    int MA = 0, NA = 0;
    int MC = 0, NC = 0;
    int MD = 0, ND = 0;
    int M1 = 0, N1 = 0;
    int lA = 0, lC = 0, lD = 0, lX = 0, lWR = 0, lWI = 0, lTYPE = 0, lMETHOD = 0;
    int lRCOND = 0, lFERR = 0, lDWORK = 0, lIWORK = 0, lBWORK = 0;

    int iOne = 1, iSize = 0, k = 0;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minLhs, maxLhs);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &MA, &NA, &lA);
    if (MA != NA)
    {
        Scierror(999, _("%s: A must be a square matrix.\n"), fname);
        return 1;
    }

    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &MD, &ND, &lD);
    if (MD != ND)
    {
        Scierror(999, _("%s: D must be a square matrix.\n"), fname);
        return 1;
    }

    GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &MC, &NC, &lC);
    if (MC != NC)
    {
        Scierror(999, _("%s: C must be a square matrix.\n"), fname);
        return 1;
    }

    if (MA != MC || MC != MD || MA != MD)
    {
        Scierror(999, _("%s: The matrices A, C and D must have the same order.\n"), fname);
        return 1;
    }

    N = MA;

    GetRhsVar(4, STRING_DATATYPE, &M1, &N1, &lTYPE);
    WANTC = (strcmp(cstk(lTYPE), "cont") == 0 || strcmp(cstk(lTYPE), "CONT") == 0);
    WANTD = (strcmp(cstk(lTYPE), "disc") == 0 || strcmp(cstk(lTYPE), "DISC") == 0);

    if (WANTC == FALSE && WANTD == FALSE)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Type must be continuous or discrete.\n"), fname, 4);
        return 1;
    }

    k = 5;
    WSCHUR = TRUE;
    if (Rhs == 5)
    {
        GetRhsVar(5, STRING_DATATYPE, &M1, &N1, &lMETHOD);
        WSCHUR = (strcmp(cstk(lMETHOD), "schr") == 0 || strcmp(cstk(lMETHOD), "SCHR") == 0);
        if (WANTC)
        {
            WSIGN  = (strcmp(cstk(lMETHOD), "sign") == 0 || strcmp(cstk(lMETHOD), "SIGN") == 0);
            if (WSCHUR == FALSE && WSIGN == FALSE)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Method must be schur or sign.\n"), fname, 5);
                return 1;
            }
        }
        else
        {
            WSIGN  = (strcmp(cstk(lMETHOD), "invf") == 0 || strcmp(cstk(lMETHOD), "INVF") == 0);
            if (WSCHUR == FALSE && WINVF == FALSE)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Method must be schur or invf.\n"), fname, 5);
                return 1;
            }
        }
        k = 6;
    }

    CreateVar(k,   MATRIX_OF_DOUBLE_DATATYPE, &N, &N, &lX);
    CreateVar(k + 1, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lWR);
    CreateVar(k + 2, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lWI);
    CreateVar(k + 3, MATRIX_OF_DOUBLE_DATATYPE, &iOne, &iOne, &lRCOND);
    CreateVar(k + 4, MATRIX_OF_DOUBLE_DATATYPE, &iOne, &iOne, &lFERR);
    iSize = Max(2 * N, N * N);
    CreateVar(k + 5, MATRIX_OF_INTEGER_DATATYPE, &iOne, &iSize, &lIWORK);
    iSize = 2 * N;
    CreateVar(k + 6, MATRIX_OF_INTEGER_DATATYPE, &iOne, &iSize, &lBWORK);

    if (WANTC)
    {
        if (WSCHUR)
        {
            LWORKMIN = 9 * N * N + 4 * N + Max(1, 6 * N);
        }
        else if (WSIGN)
        {
            LWORKMIN = 9 * N * N + 7 * N + 1;
        }
    }
    else
    {
        if (WSCHUR)
        {
            LWORKMIN = 12 * N * N + 22 * N + Max(16, 4 * N);
        }
        else if (WINVF)
        {
            LWORKMIN = 28 * N * N + 2 * N + Max(1, 2 * N);
        }
    }

    CreateVar(k + 7, MATRIX_OF_DOUBLE_DATATYPE, &iOne, &LWORKMIN, &lDWORK);

    if (WANTC)
    {
        if (WSCHUR)
        {
            C2F(riccsl)("N", &N, stk(lA), &N, "U", stk(lC), &N, stk(lD),
                        &N, stk(lX), &N, stk(lWR), stk(lWI), stk(lRCOND),
                        stk(lFERR), stk(lDWORK), &LWORKMIN, istk(lIWORK),
                        istk(lBWORK), &INFO);

            if (INFO != 0)
            {
                Scierror(999, _("%s: RICCSL exit with info = %d.\n"), fname, INFO);
                return 1;
            }
        }
        else if (WSIGN)
        {
            C2F(riccms)("N", &N, stk(lA), &N, "U", stk(lC), &N, stk(lD),
                        &N, stk(lX), &N, stk(lWR), stk(lWI), stk(lRCOND),
                        stk(lFERR), stk(lDWORK), &LWORKMIN, istk(lIWORK), &INFO);

            if (INFO != 0)
            {
                Scierror(999, _("%s: RICCMS exit with info = %d.\n"), fname, INFO);
                return 1;
            }
        }
    }
    else
    {
        if (WSCHUR)
        {
            C2F(ricdsl)("N", &N, stk(lA), &N, "U", stk(lC), &N, stk(lD),
                        &N, stk(lX), &N, stk(lWR), stk(lWI), stk(lRCOND),
                        stk(lFERR), stk(lDWORK), &LWORKMIN, istk(lIWORK),
                        istk(lBWORK), &INFO);

            if (INFO != 0)
            {
                Scierror(999, _("%s: RICDSL exit with info = %d.\n"), fname, INFO);
                return 1;
            }
        }
        else if (WINVF)
        {
            C2F(ricdmf)("N", &N, stk(lA), &N, "U", stk(lC), &N, stk(lD),
                        &N, stk(lX), &N, stk(lWR), stk(lWI), stk(lRCOND),
                        stk(lFERR), stk(lDWORK), &LWORKMIN, istk(lIWORK), &INFO);

            if (INFO != 0)
            {
                Scierror(999, _("%s: RICDMF exit with info = %d.\n"), fname, INFO);
                return 1;
            }
        }
    }

    if (Lhs == 1)
    {
        LhsVar(1) = k;
    }
    else if (Lhs == 2)
    {
        LhsVar(1) = k;
        LhsVar(2) = k + 3;
    }
    else if (Lhs == 3)
    {
        LhsVar(1) = k;
        LhsVar(2) = k + 3;
        LhsVar(3) = k + 4;
    }

    return 0;
}
