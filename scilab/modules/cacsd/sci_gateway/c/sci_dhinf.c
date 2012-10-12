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

#include "gw_slicot.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

extern int C2F(sb10dd)();
extern double C2F(dlamch)();

// [Ak,Bk,Ck,Dk,RCOND]=dhinf(A,B,C,D,ncon,nmeas,gamma)
int sci_dhinf(char *fname, unsigned long fname_len)
{
    double GAMMA = 0;
    double TOL   = 0;
    double EPS   = 0;

    int lDWORK = 0;
    int lIWORK = 0;
    int lBWORK = 0;

    int NCON     = 0;
    int NMEAS    = 0;
    int LINTWORK = 0;
    int LWORKMIN = 0;

    int MA = 0, NA = 0, lA = 0;
    int MB = 0, NB = 0, lB = 0;
    int MC = 0, NC = 0, lC = 0;
    int MD = 0, ND = 0, lD = 0;
    int M1 = 0, N1 = 0, lNCON   = 0;
    int M2 = 0, N2 = 0, lNMEAS  = 0;
    int M3 = 0, N3 = 0, lGAMMA  = 0;

    int lAK = 0, lBK = 0, lCK = 0, lDK = 0, lRCOND = 0;
    int lX = 0, lZ = 0;

    int N = 0;
    int M = 0;
    int R = 0;
    int Q = 0;

    int minrhs = 7;
    int maxrhs = 7;
    int minlhs = 4;
    int maxlhs = 5;

    int iZero  = 0;
    int iOne   = 1;
    int iEight = 8;
    int i2N    = 0;
    int info   = 0;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &MA, &NA, &lA);
    if (MA != NA)
    {
        Scierror(999, _("%s: A must be a square matrix.\n"), fname);
        return 1;
    }

    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &MB, &NB, &lB);
    if (MA != MB)
    {
        Scierror(999, _("%s: A and B must have equal number of rows.\n"), fname);
        return 1;
    }

    GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &MC, &NC, &lC);
    if (NA != NC)
    {
        Scierror(999, _("%s: A and C must have equal number of columns.\n"), fname);
        return 1;
    }

    GetRhsVar(4, MATRIX_OF_DOUBLE_DATATYPE, &MD, &ND, &lD);
    if (NB != ND)
    {
        Scierror(999, _("%s: B and D must have equal number of columns.\n"), fname);
        return 1;
    }

    if (MC != MD)
    {
        Scierror(999, _("%s: C and D must have equal number of rows.\n"), fname);
        return 1;
    }

    N = MA;
    M = NB;
    R = MC;

    if (N == 0 || M == 0 || R == 0)
    {
        CreateVar(5, MATRIX_OF_DOUBLE_DATATYPE, &iZero, &iZero, &lAK);
        CreateVar(6, MATRIX_OF_DOUBLE_DATATYPE, &iZero, &iZero, &lBK);
        CreateVar(7, MATRIX_OF_DOUBLE_DATATYPE, &iZero, &iZero, &lCK);
        CreateVar(8, MATRIX_OF_DOUBLE_DATATYPE, &iZero, &iZero, &lDK);
        CreateVar(9, MATRIX_OF_DOUBLE_DATATYPE, &iZero, &iZero, &lRCOND);

        LhsVar(1) = 5;
        LhsVar(2) = 6;
        LhsVar(3) = 7;
        LhsVar(4) = 8;
        LhsVar(5) = 9;
        return 0;
    }

    GetRhsVar(5, MATRIX_OF_INTEGER_DATATYPE, &M1, &N1, &lNCON);

    if (M1 != 1 || N1 != 1 )
    {
        Scierror(999, _("%s: NCON must be a scalar.\n"), fname);
        return 1;
    }

    NCON = *istk(lNCON);

    GetRhsVar(6, MATRIX_OF_INTEGER_DATATYPE, &M2, &N2, &lNMEAS);
    if (M2 != 1 || N2 != 1 )
    {
        Scierror(999, _("%s: NMEAS must be a scalar.\n"), fname);
        return 1;
    }

    NMEAS = *istk(lNMEAS);

    GetRhsVar(7, MATRIX_OF_DOUBLE_DATATYPE, &M3, &N3, &lGAMMA);
    if (M3 != 1 || N3 != 1 )
    {
        Scierror(999, _("%s: GAMMA must be a scalar.\n"), fname);
        return 1;
    }

    GAMMA = *stk(lGAMMA);
    EPS = C2F(dlamch)("e", 1L);
    TOL = sqrt(EPS);

    CreateVar(8,  MATRIX_OF_DOUBLE_DATATYPE, &N, &N, &lAK);
    CreateVar(9,  MATRIX_OF_DOUBLE_DATATYPE, &N, &NMEAS, &lBK);
    CreateVar(10, MATRIX_OF_DOUBLE_DATATYPE, &NCON, &N, &lCK);
    CreateVar(11, MATRIX_OF_DOUBLE_DATATYPE, &NCON, &NMEAS, &lDK);
    CreateVar(12, MATRIX_OF_DOUBLE_DATATYPE, &N, &N, &lX);
    CreateVar(13, MATRIX_OF_DOUBLE_DATATYPE, &N, &N, &lZ);
    CreateVar(14, MATRIX_OF_DOUBLE_DATATYPE, &iEight, &iOne, &lRCOND);
    LINTWORK = Max(2 * Max(NCON, N), Max(M, Max(NCON + NMEAS, N * N)));
    CreateVar(15, MATRIX_OF_INTEGER_DATATYPE, &LINTWORK, &iOne, &lIWORK);
    i2N = 2 * N;
    CreateVar(16, MATRIX_OF_INTEGER_DATATYPE, &i2N, &iOne, &lBWORK);

    Q = Max( M - NCON, Max(NCON, Max(R - NMEAS, NMEAS)));

    LWORKMIN =  Max((N + Q) * (N + Q + 6),
                    13 * N * N + M * M + 2 * Q * Q + N * (M + Q) +
                    Max(M * (M + 7 * N), 2 * Q * (8 * N + M + 2 * Q)) +
                    6 * N +
                    Max(14 * N + 23, Max(16 * N, Max(2 * N + Max(M, 2 * Q) , 3 * Max(M, 2 * Q)))));

    CreateVar(17, MATRIX_OF_DOUBLE_DATATYPE, &iOne, &LWORKMIN, &lDWORK);

    C2F(sb10dd)(&N, &M, &R, &NCON, &NMEAS, &GAMMA, stk(lA), &N,
                stk(lB), &N, stk(lC), &R, stk(lD), &R, stk(lAK),
                &N, stk(lBK), &N, stk(lCK), &NCON, stk(lDK),
                &NCON, stk(lX), &N, stk(lZ), &N, stk(lRCOND), &TOL,
                istk(lIWORK), stk(lDWORK), &LWORKMIN, istk(lBWORK), &info);

    if (info != 0)
    {
        Scierror(999, _("%s: SB10DD exit with info = %d.\n"), fname, info);
        return 1;
    }

    LhsVar(1) = 8;
    LhsVar(2) = 9;
    LhsVar(3) = 10;
    LhsVar(4) = 11;
    if (Lhs == 5)
    {
        LhsVar(5) = 14;
    }

    return 0;
}




