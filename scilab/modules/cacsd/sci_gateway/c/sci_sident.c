/*
* Scilab (http://www.scilab.org/) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

//SIDENT.F - Gateway function for computation of a discrete-time
//           state-space realization (A,B,C,D) and Kalman gain
//           using SLICOT routine IB01BD.
//
//RELEASE 4.0, WGS COPYRIGHT 2000.
//
//scilab call:
//  [(A,C)(,B(,D))(,K,Q,Ry,S,rcnd)] = sident(meth,job,s,n,l,R(,tol,t,A,
//                                           C,printw))
//
//Purpose:
//  To compute a state-space realization (A,B,C,D) and the Kalman
//  predictor gain K of a discrete-time system, given the system
//  order and the relevant part of the R factor of the concatenated
//  block-Hankel matrices, using subspace identification techniques
//  (MOESP and N4SID).
//
//Input parameters:
//  meth  - integer option to determine the method to use:
//          = 1 : MOESP method with past inputs and outputs;
//          = 2 : N4SID method;
//          = 3 : combined method: A and C via MOESP, B and D via N4SID.
//  job   - integer option to determine the calculation to be performed:
//          = 1 : compute all system matrices, A, B, C, D;
//          = 2 : compute the matrices A and C only;
//          = 3 : compute the matrix B only;
//          = 4 : compute the matrices B and D only.
//  s     - the number of block rows in the processed input and output
//          block Hankel matrices.  s > 0.
//  n     - the order of the system.
//  l     - the number of system outputs.
//  R     - the 2*(m+l)*s-by-2*(m+l)*s part of  R  contains the
//          processed upper triangular factor  R  from the
//          QR factorization of the concatenated block-Hankel matrices,
//          and further details needed for computing system matrices.
//          (Above, m denotes the number of system inputs, determined
//          by s, l, and the size of R.)
//  tol   - (optional) tolerance used for estimating the rank of
//          matrices. if  tol > 0,  the given value of  tol  is
//          used as a lower bound for the reciprocal condition number;
//          an m-by-n matrix whose estimated condition number is less
//          than  1/tol  is considered to be of full rank.
//          Default:    m*n*epsilon_machine where epsilon_machine is
//          the relative machine precision.
//  t     - (optional) the total number of samples used for calculating
//          the covariance matrices.  Either  t = 0, or  t >= 2*(m+l)*s.
//          This parameter is not needed if the covariance matrices
//          and/or the Kalman predictor gain matrix are not desired.
//          if t = 0, K, Q, Ry, and S are not computed.
//          Default:    t = 0.
//  A     - (optional) the n-by-n system state matrix A.
//          This parameter is needed if meth >= 2 and job >= 3.
//  C     - (optional) the l-by-n system output matrix C.
//          This parameter is needed if meth >= 2 and job >= 3.
//  printw- (optional) switch for printing the warning messages.
//          = 1:  print warning messages;
//          = 0:  do not print warning messages.
//          Default:    printw = 0.
//
//Output parameters:
//  A     - if job <= 2, the n-by-n system state matrix A.
//  C     - if job <= 2, the l-by-n system output matrix C.
//  B     - if job <> 2, the n-by-m system input matrix B.
//  D     - if job = 1 or 4, the l-by-m system matrix D.
//  K     - (optional) the n-by-l Kalman predictor gain matrix K.
//  Q     - (optional) the n-by-n positive semidefinite state covariance
//          matrix used as state weighting matrix when computing the
//          Kalman gain.
//  Ry    - (optional) the l-by-l positive (semi)definite output
//          covariance matrix used as output weighting matrix when
//          computing the Kalman gain.
//  S     - (optional) the n-by-l state-output cross-covariance matrix
//          used as cross-weighting matrix when computing the Kalman
//          gain.
//  rcnd  - (optional) vector of length lr, containing estimates of the
//          reciprocal condition numbers of the matrices involved in
//          rank decisions, least squares or Riccati equation solutions,
//          where lr = 4,  if Kalman gain matrix K is not required, and
//                lr = 12, if Kalman gain matrix K is required.
//
//Contributor:
//  V. Sima, Research Institute for Informatics, Bucharest, Oct. 1999.
//
//Revisions:
//  V. Sima, May 2000, July 2000.

#include <string.h>
#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "BOOL.h"
#include "MALLOC.h"
#include "gw_cacsd0.h"

int sci_sident(char* fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iOne = 1;

    int iTASK = 0;
    char cMETH = 'C';
    int iIJOB = 0;
    char cJOB = 'D';
    int iNOBR = 0;
    int iN = 0;
    int iL = 0;
    int* piAddrR = NULL;
    int iNR = 0;
    int iNCOL = 0;
    double* pdblR = NULL;
    int iM = 0;
    double dblTOL = 0;
    int iNSMPL = 0;
    char cJOBCK = 'N';
    int iMA = 0;
    int iNA = 0;
    int iSizeA = 0;
    double* pdblA = NULL;
    double* pdblC = NULL;
    int iPRINTW = 0;
    BOOL bPRINTW = FALSE;

    int iMNOBR = 0;
    int iLNOBR = 0;
    int iMNOBRN = 0;
    int iLDUNN = 0;
    int iNPL = 0;
    int iN2 = 0;
    int iNN = 0;
    int iNL = 0;
    int iLL = 0;

    int iLDA = 0;
    int iLDB = 0;
    int iLDC = 0;
    int iLDD = 0;
    int iLDO = 0;
    int iLDR = 0;

    int iLDK = 0;
    int iLDQ = 0;
    int iLDS = 0;
    int iLDRY = 0;
    int iLBWORK = 0;
    int iLIWORK = 0;
    int iLDWORK = 0;
    int iID = 0;
    int iIAW = 0;

    int iRhs = nbInputArgument(pvApiCtx);
    int iLhs = nbOutputArgument(pvApiCtx);

    CheckInputArgumentAtLeast(pvApiCtx, 6);
    CheckOutputArgumentAtLeast(pvApiCtx, 1);

    // meth
    CHECK_PARAM(pvApiCtx, 1);
    iTASK = getIntegerValue(pvApiCtx, 1);

    switch (iTASK)
    {
        case 1 :
            cMETH = 'M';
            break;
        case 2 :
            cMETH = 'N';
            break;
        default :
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "1", "2");
            return 0;
    }

    // job
    CHECK_PARAM(pvApiCtx, 2);
    iIJOB = getIntegerValue(pvApiCtx, 2);

    switch (iIJOB)
    {
        case 1 :
            cJOB = 'A';
            break;
        case 2 :
            cJOB = 'C';
            break;
        case 3 :
            cJOB = 'B';
            break;
        default:
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 2, "1", "2", "3");
            return 0;
    }


    // s
    CHECK_PARAM(pvApiCtx, 3);
    iNOBR = getIntegerValue(pvApiCtx, 3);
    if (iNOBR < 1)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), fname, 3);
        return 0;
    }

    // n
    CHECK_PARAM(pvApiCtx, 4);
    iN = getIntegerValue(pvApiCtx, 4);
    if (iN < 1)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), fname, 4);
        return 0;
    }

    if (iN >= iNOBR)
    {
        Scierror(999, _("%s: The order should be at most %d.\n"), fname, 4, iNOBR - 1);
        return 0;
    }


    // l
    CHECK_PARAM(pvApiCtx, 5);
    iL = getIntegerValue(pvApiCtx, 5);
    if (iL < 1)
    {
        Scierror(999, _("%s: The system has no outputs\n"), fname);
        return 0;
    }

    // R(nr,nr)

    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddrR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 6);
        return 0;
    }

    getMatrixOfDouble(pvApiCtx, piAddrR, &iNR, &iNCOL, &pdblR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 6);
        return 0;
    }


    if (iNR < 2 * iL)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Must have at least %d rows expected.\n"), fname, 6, 2 * iL);
        return 0;
    }


    if (iNCOL < iNR)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Must have at least %d columns expected.\n"), fname, 6, iNCOL);
        return 0;
    }

    // m
    iM = iNR / (2 * iNOBR) - iL;

    // tol
    if (iRhs >= 6)
    {
        int* piAddr = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
            return 0;
        }

        getScalarDouble(pvApiCtx, piAddr, &dblTOL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
            return 0;
        }
    }


    // t
    if (iRhs >= 7)
    {
        cJOBCK = 'K';
        CHECK_PARAM(pvApiCtx, 8);
        iNSMPL = getInputArgumentType(pvApiCtx, 8);

        if (iNSMPL != 0 && iNSMPL < iNR)
        {
            Scierror(999, _("%s: The number of samples should be at least %d\n"), fname, iNR);
            return 0;
        }
        else if (iNSMPL == 0)
        {
            cJOBCK = 'N';
        }
    }

    // A(n,n)

    if (iTASK >= 2 && iIJOB >= 3)
    {
        int* piAddr = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddr, &iMA, &iNA, &pdblA);
        iSizeA = iMA * iNA;

        if (iMA != iN || iNA != iN)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %dx%d expected.\n"), fname, 9, iN, iN);
            return 0;
        }

        // C(1,n)
        sciErr = getVarAddressFromPosition(pvApiCtx, 10, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 10);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddr, &iMA, &iNA, &pdblC);
        iSizeA = iMA * iNA;

        if (iMA != iL)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Must have %d rows expected.\n"), fname, 10, iL);
            return 0;
        }

        if (iNA != iN)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Must have %d columns expected.\n"), fname, 10, iN);
            return 0;
        }
    }

    // printw
    if (iRhs >= 11)
    {
        int iPrintw = 0;
        CHECK_PARAM(pvApiCtx, 11);
        iPRINTW = getIntegerValue(pvApiCtx, 11);

        switch (iPRINTW)
        {
            case 1 :
                bPRINTW = TRUE;
                break;
            case 0 :
                bPRINTW = FALSE;
                break;
            default :
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 11, "0", "1");
                return 0;
        }
    }

    // Determine the lengths of working arrays.
    // The value for LDWORK is the minimum value needed by IB01BD for each
    // method and algorithm implemented.  Using a larger value could
    // increase the efficiency.
    iMNOBR = iM * iNOBR;
    iLNOBR = iL * iNOBR;
    iMNOBRN = iMNOBR + iN;
    iLDUNN = (iLNOBR - iL) * iN;
    iNPL = iN + iL;
    iN2 = iN + iN;
    iNN = iN * iN;
    iNL = iN * iL;
    iLL = iL * iL;

    iLDA = Max(1, iN);
    iLDB = iLDA;
    iLDC = Max(1, iL);
    iLDD = iLDC;
    iLDO = iLNOBR;
    iLDR = iNR;
    if (iNSMPL != 0)
    {
        iLDK = iLDA;
        iLDQ = iLDA;
        iLDS = iLDA;
        iLDRY = iLDC;
        iLBWORK = iN2;
    }
    else
    {
        iLDK = 1;
        iLDQ = 1;
        iLDS = 1;
        iLDRY = 1;
        iLBWORK = 1;
    }

    iLIWORK = iMNOBR + iN;
    if (iTASK == 1)
    {
        iLIWORK = Max(iLIWORK, iLNOBR);
    }
    else if (iTASK == 2)
    {
        iLIWORK = Max(iLIWORK, iM * iNPL);
    }
    else
    {
        iLIWORK = Max(iLIWORK, Max(iLNOBR, iM * iNPL));
    }
    if (iNSMPL >= 0)
    {
        iLIWORK = Max(iLIWORK, iNN);
    }

    iIAW = 0;
    iLDWORK = iLDUNN + 4 * iN;
    if (iTASK == 1)
    {
        iID = 0;
    }
    else
    {
        iID = iN;
    }

    if (iTASK != 2 && iIJOB <= 2)
    {
        iLDWORK = Max(iLDWORK, Max(2 * iLDUNN + iN2, iLDUNN + iNN + 7 * iN));
    }

    if ((iM > 0 && iIJOB != 2) || iTASK >= 2)
    {
        iLDWORK = Max(iLDWORK, 2 * iLDUNN + iNN + iID + 7 * iN);
        if (iTASK == 1)
        {
            iLDWORK = Max(iLDWORK, Max(iLDUNN + iN + 6 * iMNOBR, iLDUNN + iN + Max(iL + iMNOBR, iLNOBR + Max (3 * iLNOBR, iM))));
        }
    }
    else
    {
        if (iTASK != 2)
        {
            iIAW = iN + iNN;
        }
    }

    if (iTASK != 1 || iNSMPL > 0)
    {
        iLDWORK = Max(iLDWORK,
                      Max(iLDUNN + iIAW + iN2 + Max(5 * iN, iLNOBR + 2 * iMNOBR + iL),
                          Max(iID + 4 * iMNOBRN, iID + iMNOBRN + iNPL)));
    }

    ReturnArguments(pvApiCtx);
    return 0;
}