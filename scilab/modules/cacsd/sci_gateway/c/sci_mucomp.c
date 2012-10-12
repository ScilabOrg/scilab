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

extern int C2F(ab13md)();

//     [bound,D,G] = mucomp(Z,K,T)
//     [bound,D] = mucomp(Z,K,T)
//     bound = mucomp(Z,K,T)

int sci_mucomp(char *fname, unsigned long fname_len)
{
    int lD = 0, lG = 0, lK = 0, lT = 0, lX = 0, lZ = 0;
    int lBOUND = 0, lIWORK = 0, lRWORK = 0, lZWORK = 0;
    int LRWRK = 0, LZWRKMIN = 0;
    int M = 0, N = 0;
    lZ = 0;
    int M1 = 0, N1 = 0, M2 = 0, N2 = 0;
    int INFO = 0;

    int minrhs = 3;
    int maxrhs = 3;
    int minLhs = 1;
    int maxLhs = 3;

    int iOne  = 1;
    int iSize = 0;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minLhs, maxLhs);

    GetRhsVar(1, MATRIX_OF_COMPLEX_DATATYPE, &M, &N, &lZ);
    if (M != N)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), fname, 2);
        return 1;
    }

    if (N == 0)
    {
        if (Lhs == 1)
        {
            CreateVar(2, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lBOUND);
            LhsVar(1) = 2;
            return 0;
        }
        else if (Lhs == 2)
        {
            CreateVar(2, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lBOUND);
            CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lD);
            LhsVar(1) = 2;
            LhsVar(2) = 3;
            return 0;
        }
        else if (Lhs == 3)
        {
            CreateVar(2, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lBOUND);
            CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lD);
            CreateVar(4, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lG);
            LhsVar(1) = 2;
            LhsVar(2) = 3;
            LhsVar(3) = 4;
            return 0;
        }
    }

    GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &M1, &N1, &lK);
    GetRhsVar(3, MATRIX_OF_INTEGER_DATATYPE, &M2, &N2, &lT);

    if (M1 * N1 != M2 * N2)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same size expected.\n"), fname, 2);
        return 1;
    }

    M = M1 * N1;

    CreateVar(4, MATRIX_OF_DOUBLE_DATATYPE, &iOne, &iOne, &lBOUND);
    CreateVar(5, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lD);
    CreateVar(6, MATRIX_OF_DOUBLE_DATATYPE, &N, &iOne, &lG);
    iSize = 2 * N - 1;
    CreateVar(7, MATRIX_OF_DOUBLE_DATATYPE, &iSize, &iOne, &lX);
    iSize = 4 * N - 2;
    CreateVar(8, MATRIX_OF_INTEGER_DATATYPE, &iSize, &iOne, &lIWORK);
    LRWRK = 2 * N * N * N + 9 * N * N +  44 * N - 11;
    CreateVar(9, MATRIX_OF_DOUBLE_DATATYPE, &LRWRK, &iOne, &lRWORK);
    LZWRKMIN = 6 * N * N * N + 12 * N * N + 12 * N - 3;
    CreateVar(10, MATRIX_OF_COMPLEX_DATATYPE, &iOne, &LZWRKMIN, &lZWORK);

    C2F(ab13md)("N", &N, zstk(lZ), &N, &M, istk(lK), istk(lT),
                stk(lX), stk(lBOUND), stk(lD), stk(lG), istk(lIWORK),
                stk(lRWORK), &LRWRK, zstk(lZWORK), &LZWRKMIN, &INFO);

    if (INFO != 0)
    {
        Scierror(999, _("%s: AB13MD exit with info = %d.\n"), fname, INFO);
        return 1;
    }

    if (Lhs == 1)
    {
        LhsVar(1) = 4;
    }
    else if (Lhs == 2)
    {
        LhsVar(1) = 4;
        LhsVar(2) = 5;
    }
    else if (Lhs == 3)
    {
        LhsVar(1) = 4;
        LhsVar(2) = 5;
        LhsVar(3) = 6;
    }

    return 0;
}
