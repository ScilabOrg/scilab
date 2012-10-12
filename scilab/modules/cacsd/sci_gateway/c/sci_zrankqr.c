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

extern int C2F(icopy)();
extern int C2F(zcopy)();
extern int C2F(zb03od)();
extern int C2F(zungqr)();

int sci_zrankqr(char *fname, unsigned long fname_len)
{
    int minrhs = 1;
    int maxrhs = 3;
    int minlhs = 1;
    int maxlhs = 5;

    char JOBQR      = 'Q';
    double SLVMAX   = 0.;
    double RCONDdef = 0.;

    int A       = 1;
    int RCOND   = 2;
    int JPVT    = 3;
    int SVAL    = 4;
    int TAU     = 5;
    int RANK    = 6;
    int R       = 7;
    int Q       = 8;
    int RWORK   = 9;
    int DWORK   = 10;

    int LDWORKMIN = 0, LDA = 0, INFO = 0, IRANK = 0, IR = 0;
    int II = 0, NB = 0, NB1 = 0, IR1 = 0;

    int ptrA        = 0;
    int ptrRCOND    = 0;
    int ptrJPVT     = 0;
    int ptrSVAL     = 0;
    int ptrTAU      = 0;
    int ptrR        = 0;
    int ptrQ        = 0;
    int ptrDWORK    = 0;
    int ptrRWORK    = 0;
    int ptrRANK     = 0;

    int mR = 0, nR = 0;
    int mJ = 0, nJ = 0;
    int M  = 0, N  = 0;

    int iZero = 0, iOne = 1, iThree = 3, iMin = 0, iSize = 0;

    int i = 0;

    doublecomplex ZERO;
    ZERO.r = 0.;
    ZERO.i = 0.;

    CheckLhs(minlhs, maxlhs);
    CheckRhs(minrhs, maxrhs);

    GetRhsVar(A, MATRIX_OF_COMPLEX_DATATYPE, &M, &N, &ptrA);

    if (Rhs == 1)
    {
        // rankqr(A)
        // create RCOND=0.0,JPVT=[0,..,0]
        CreateVar(RCOND, MATRIX_OF_DOUBLE_DATATYPE, &iOne, &iOne, &ptrRCOND);
        *stk(ptrRCOND) = RCONDdef;
        CreateVar(JPVT, MATRIX_OF_INTEGER_DATATYPE, &iOne, &N, &ptrJPVT);
        C2F(icopy)(&N, &iZero, &iZero, istk(ptrJPVT), &iOne);
    }
    else if (Rhs == 2)
    {
        // rankqr(A,RCOND)
        // get RCOND, create JPVT=[0,..,0]
        GetRhsVar(RCOND, MATRIX_OF_DOUBLE_DATATYPE, &mR, &nR, &ptrRCOND);
        if (mR * nR != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, RCOND);
            return 1;
        }

        CreateVar(JPVT, MATRIX_OF_INTEGER_DATATYPE, &iOne, &N, &ptrJPVT);
        C2F(icopy)(&N, &iZero, &iZero, istk(ptrJPVT), &iOne);
    }
    else if (Rhs == 3)
    {
        // rankqr(A,RCOND,JPVT)
        // get  RCOND, JPVT
        GetRhsVar(RCOND, MATRIX_OF_DOUBLE_DATATYPE, &mR, &nR, &ptrRCOND);
        if (mR * nR != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, RCOND);
            return 1;
        }

        GetRhsVar(JPVT, MATRIX_OF_INTEGER_DATATYPE, &mJ, &nJ, &ptrJPVT);
        if (mJ * nJ != N)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Same column dimension as input argument #%d expected.\n"), fname, JPVT, A);
            return 1;
        }
    }

    // Creating  SVAL,TAU,RANK,R,Q,RWORK,DWORK
    CreateVar(SVAL, MATRIX_OF_DOUBLE_DATATYPE, &iOne, &iThree, &ptrSVAL);
    iMin = Min(M, N);
    CreateVar(TAU, MATRIX_OF_COMPLEX_DATATYPE, &iOne, &iMin, &ptrTAU);
    CreateVar(RANK, MATRIX_OF_INTEGER_DATATYPE, &iOne, &iOne, &ptrRANK);
    CreateVar(R, MATRIX_OF_COMPLEX_DATATYPE, &M, &N, &ptrR);
    CreateVar(Q, MATRIX_OF_COMPLEX_DATATYPE, &M, &M, &ptrQ);
    iSize = 2 * N;
    CreateVar(RWORK, MATRIX_OF_DOUBLE_DATATYPE, &iOne, &iSize, &ptrRWORK);

    LDWORKMIN = Max(1, 3 * N);
    CreateVar(DWORK, MATRIX_OF_COMPLEX_DATATYPE, &iOne, &LDWORKMIN, &ptrDWORK);
    LDA = Max(1, M);

    C2F(zb03od)(&JOBQR, &M, &N, zstk(ptrA), &LDA, istk(ptrJPVT),
                stk(ptrRCOND), &SLVMAX, zstk(ptrTAU), istk(ptrRANK),
                stk(ptrSVAL), zstk(ptrDWORK), &LDWORKMIN, stk(ptrRWORK), &INFO);

    if (INFO != 0)
    {
        Scierror(999, _("%s: ZB03OD exit with info = %d.\n"), fname, INFO);
        return 1;
    }

    //  Save R (=current A)
    iSize = M * N;
    C2F(zcopy)(&iSize, zstk(ptrA), &iOne, zstk(ptrR), &iOne);

    //  Set to zero lower trapezoidal part of R:
    IRANK = *istk(ptrRANK);
    IR = IRANK - M;
    for (i = 0; i < N; i++)
    {
        II = II + 1;
        NB = M - IRANK;
        IR = IR + M;
        NB1 = NB;
        IR1 = IR;
        if (i < IRANK)
        {
            IR1 = IR - (IRANK - i);
            NB1 = NB + (IRANK - i);
        }

        C2F(zcopy)(&NB1, &ZERO, &iZero, zstk(ptrR + IR1), &iOne);
    }

    // Make Q
    if (M <= N)
    {
        // A is fat
        C2F(zungqr)(&M, &M, &M, zstk(ptrA), &LDA, zstk(ptrTAU),
                    zstk(ptrDWORK), &LDWORKMIN, &INFO);
        iSize = M * M;
        C2F(zcopy)(&iSize, zstk(ptrA), &iOne, zstk(ptrQ), &iOne);
    }
    else
    {
        // A is tall => Q=[A,0]
        iSize = M * N;
        C2F(zcopy)(&iSize, zstk(ptrA), &iOne, zstk(ptrQ), &iOne);
        iSize = M * (M - N);
        C2F(zcopy)(&iSize, &ZERO, &iZero, zstk(ptrQ + M * N), &iOne);
        iMin = Min(M, N);
        C2F(zungqr)(&M, &M, &iMin, zstk(ptrQ), &LDA, zstk(ptrTAU),
                    zstk(ptrDWORK), &LDWORKMIN, &INFO);
    }

    if (INFO != 0)
    {
        Scierror(999, _("%s: ZUNGQR exit with info = %d.\n"), fname, INFO);
        return 1;
    }

    // [Q,R,JPVT,RANK,SVAL]=rankqr(A,[RCOND,JPVT])
    LhsVar(1) = Q;
    LhsVar(2) = R;
    LhsVar(3) = JPVT;
    LhsVar(4) = RANK;
    LhsVar(5) = SVAL;

    return 0;
}
