/*
 *   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
 *   <bruno.pincon@iecn.u-nancy.fr>
 *   contributor:  Antonio Manoel Ferreria Frasson, Universidade Federal do
 *                 Espírito Santo, Brazil. <frasson@ele.ufes.br>.
 *
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * PURPOSE: Scilab interfaces routines onto the UMFPACK sparse solver
 * (Tim Davis) and onto the TAUCS snmf choleski solver (Sivan Teledo)
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 */

/*------------------------------------------------------------+
  | 1) Interface code for the direct solving :                  |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   Case 1 :                                                  |
  |   [x] = umfpack(A,"\",b) -> solve directly Ax=b             |
  |                                                             |
  |   Case 2 :                                                  |
  |   [x] = umfpack(b,"/",A) -> solve directly xA=b <=> A'x'=b' |
  |                                                             |
  |   Var description (to complete...)                          |
  |   ---------------                                           |
  |      mA : number of rows of the matrix A                    |
  |      nA : number of columns of A                            |
  |       A : pointer to a sparse matrix struct                 |
  |                                                             |
  |      Require mA == nA (matrix must be square), mA > 0       |
  |                                                             |
  |      mb : number of rows of the vector b                    |
  |      nb : number of columns of the vector b                 |
  |      pdblBR : real part of b                                |
  |      pdblBI : imaginary part of b                           |
  |                                                             |
  |      Require mb = mA , nb >= 1 in Case 1                    |
  |              mb >= 1 , nb = mA in Case 2                    |
  |                                                             |
  +------------------------------------------------------------*/
#include "api_scilab.h"
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "Scierror.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"
#include "MALLOC.h"

int sci_umfpack(char* fname, unsigned long l)
{
    SciErr sciErr;

    int mb      = 0;
    int nb      = 0;
    int i       = 0;
    int num_A   = 0;
    int num_b   = 0;
    int mW      = 0;
    int Case    = 0;
    int stat    = 0;

    SciSparse AA;
    CcsSparse A;

    int* piAddrA = NULL;
    int* piAddr2 = NULL;
    int* piAddrB = NULL;

    double* pdblBR = NULL;
    double* pdblBI = NULL;
    double* pdblXR = NULL;
    double* pdblXI = NULL;

    int iComplex = 0;

    int mA              = 0; // rows
    int nA              = 0; // cols
    int iNbItem         = 0;
    int* piNbItemRow    = NULL;
    int* piColPos       = NULL;
    double* pdblSpReal  = NULL;
    double* pdblSpImg   = NULL;

    /* umfpack stuff */
    double Info[UMFPACK_INFO];
    double* Control = NULL;
    void* Symbolic  = NULL;
    void* Numeric   = NULL;
    int* Wi         = NULL;
    double* W       = NULL;
    char* pStr      = NULL;
    int iType2      = 0;
    int iTypeA      = 0;
    int iTypeB      = 0;

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    /* First get arg #2 : a string of length 1 */
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddr2, &iType2);
    if (sciErr.iErr || iType2 != sci_strings)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 1;
    }

    getAllocatedSingleString(pvApiCtx, piAddr2, &pStr);

    /* select Case 1 or 2 depending (of the first char of) the string ... */
    if (pStr[0] == '\\') // compare pStr[0] with '\'
    {
        Case  = 1;
        num_A = 1;
        num_b = 3;
    }
    else if (pStr[0] == '/')
    {
        Case  = 2;
        num_A = 3;
        num_b = 1;
    }
    else
    {
        Scierror(999, _("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"), fname, 2, "\\", "/");
        return 1;
    }

    /* get A */
    sciErr = getVarAddressFromPosition(pvApiCtx, num_A, &piAddrA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddrA, &iTypeA);
    if (sciErr.iErr || iTypeA != sci_sparse)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), fname, 1);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddrA))
    {
        AA.it = 1;
        iComplex = 1;
        sciErr = getComplexSparseMatrix(pvApiCtx, piAddrA, &mA, &nA, &iNbItem, &piNbItemRow, &piColPos, &pdblSpReal, &pdblSpImg);
    }
    else
    {
        AA.it = 0;
        sciErr = getSparseMatrix(pvApiCtx, piAddrA, &mA, &nA, &iNbItem, &piNbItemRow, &piColPos, &pdblSpReal);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // fill struct sparse
    AA.m     = mA;
    AA.n     = nA;
    AA.nel   = iNbItem;
    AA.mnel  = piNbItemRow;
    AA.icol  = piColPos;
    AA.R     = pdblSpReal;
    AA.I     = pdblSpImg;

    if ( mA != nA || mA < 1 )
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, num_A);
        return 1;
    }

    /* get B*/
    sciErr = getVarAddressFromPosition(pvApiCtx, num_b, &piAddrB);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddrB, &iTypeB);
    if (sciErr.iErr || iTypeB != sci_matrix)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), fname, 3);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddrB))
    {
        iComplex = 1;
        sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddrB, &mb, &nb, &pdblBR, &pdblBI);
    }
    else
    {
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrB, &mb, &nb, &pdblBR);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if ( (Case == 1 && ( mb != mA || nb < 1 )) || (Case == 2 && ( nb != mA || mb < 1 )) )
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, num_b);
        return 1;
    }

    SciSparseToCcsSparse(&AA, &A);

    /* allocate memory for the solution x */
    if (iComplex)
    {
        sciErr = allocComplexMatrixOfDouble(pvApiCtx, 4, mb, nb, &pdblXR, &pdblXI);
    }
    else
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, 4, mb, nb, &pdblXR);
    }

    if (sciErr.iErr)
    {
        freeCcsSparse(A);
        printError(&sciErr, 0);
        return 1;
    }

    /* allocate memory for umfpack_di_wsolve usage or umfpack_zi_wsolve usage*/
    Wi = (int*)MALLOC(mA * sizeof(int));

    if (A.it == 1)
    {
        mW = 10 * mA;
    }
    else
    {
        mW = 5 * mA;
    }

    W = (double*)MALLOC(mW * sizeof(double));

    if (A.it == 1  &&  pdblBI == NULL)
    {
        int iSize = mb * nb * sizeof(double);
        pdblBI = (double*)MALLOC(iSize);
        memset(pdblBI, 0x00, iSize);
    }

    /* Now calling umfpack routines */
    if (A.it == 1)
    {
        stat = umfpack_zi_symbolic(mA, nA, A.p, A.irow, A.R, A.I, &Symbolic, Control, Info);
    }
    else
    {
        stat = umfpack_di_symbolic(mA, nA, A.p, A.irow, A.R, &Symbolic, Control, Info);
    }

    if ( stat  != UMFPACK_OK )
    {
        freeCcsSparse(A);
        Scierror(999, _("%s: An error occurred: %s: %s\n"), fname, _("symbolic factorization"), UmfErrorMes(stat));
        return 1;
    }

    if (A.it == 1)
    {
        stat = umfpack_zi_numeric(A.p, A.irow, A.R, A.I, Symbolic, &Numeric, Control, Info);
    }
    else
    {
        stat = umfpack_di_numeric(A.p, A.irow, A.R, Symbolic, &Numeric, Control, Info);
    }

    if (A.it == 1)
    {
        umfpack_zi_free_symbolic(&Symbolic);
    }
    else
    {
        umfpack_di_free_symbolic(&Symbolic);
    }

    if ( stat  != UMFPACK_OK )
    {
        if (A.it == 1)
        {
            umfpack_zi_free_numeric(&Numeric);
        }
        else
        {
            umfpack_di_free_numeric(&Numeric);
        }
        freeCcsSparse(A);
        Scierror(999, _("%s: An error occurred: %s: %s\n"), fname, _("numeric factorization"), UmfErrorMes(stat));
        return 1;
    }


    if ( Case == 1 )   /*  x = A\b  <=> Ax = b */
    {
        if (A.it == 0)
        {
            for ( i = 0 ; i < nb ; i++ )
            {
                umfpack_di_wsolve(UMFPACK_A, A.p, A.irow, A.R, &pdblXR[i * mb], &pdblBR[i * mb],
                                  Numeric, Control, Info, Wi, W);
            }

            if (isVarComplex(pvApiCtx, piAddrB))
            {
                for ( i = 0 ; i < nb ; i++ )
                {
                    umfpack_di_wsolve(UMFPACK_A, A.p, A.irow, A.R, &pdblXI[i * mb], &pdblBI[i * mb],
                                      Numeric, Control, Info, Wi, W);
                }
            }
        }
        else /*  A.it == 1  */
        {
            for ( i = 0 ; i < nb ; i++ )
            {
                umfpack_zi_wsolve(UMFPACK_A, A.p, A.irow, A.R, A.I, &pdblXR[i * mb], &pdblXI[i * mb],
                                  &pdblBR[i * mb], &pdblBI[i * mb], Numeric, Control, Info, Wi, W);
            }
        }
    }
    else  /* Case == 2,   x = b/A  <=> x A = b <=> A.'x.' = b.' */
    {
        if (A.it == 0)
        {
            TransposeMatrix(pdblBR, mb, nb, pdblXR);    /* put b in x (with transposition) */
            for ( i = 0 ; i < mb ; i++ )
            {
                umfpack_di_wsolve(UMFPACK_At, A.p, A.irow, A.R, &pdblBR[i * nb], &pdblXR[i * nb],
                                  Numeric, Control, Info, Wi, W);      /* the solutions are in br */
            }

            TransposeMatrix(pdblBR, nb, mb, pdblXR);         /* put now br in xr with transposition */

            if (isVarComplex(pvApiCtx, piAddrB))
            {
                TransposeMatrix(pdblBI, mb, nb, pdblXI);    /* put b in x (with transposition) */
                for ( i = 0 ; i < mb ; i++ )
                {
                    umfpack_di_wsolve(UMFPACK_At, A.p, A.irow, A.R, &pdblBI[i * nb], &pdblXI[i * nb],
                                      Numeric, Control, Info, Wi, W);      /* the solutions are in bi */
                }
                TransposeMatrix(pdblBI, nb, mb, pdblXI);         /* put now bi in xi with transposition */
            }
        }
        else /*  A.it==1  */
        {
            TransposeMatrix(pdblBR, mb, nb, pdblXR);
            TransposeMatrix(pdblBI, mb, nb, pdblXI);
            for ( i = 0 ; i < mb ; i++ )
            {
                umfpack_zi_wsolve(UMFPACK_Aat, A.p, A.irow, A.R, A.I, &pdblBR[i * nb], &pdblBI[i * nb],
                                  &pdblXR[i * nb], &pdblXI[i * nb], Numeric, Control, Info, Wi, W);
            }
            TransposeMatrix(pdblBR, nb, mb, pdblXR);
            TransposeMatrix(pdblBI, nb, mb, pdblXI);
        }
    }

    if (A.it == 1)
    {
        umfpack_zi_free_numeric(&Numeric);
    }
    else
    {
        umfpack_di_free_numeric(&Numeric);
    }

    freeCcsSparse(A);

    AssignOutputVariable(pvApiCtx, 1) = 4;
    ReturnArguments(pvApiCtx);
    return 0;
}
