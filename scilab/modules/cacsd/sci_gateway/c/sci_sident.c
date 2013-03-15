/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
//          matrices. If  tol > 0,  then the given value of  tol  is
//          used as a lower bound for the reciprocal condition number;
//          an m-by-n matrix whose estimated condition number is less
//          than  1/tol  is considered to be of full rank.
//          Default:    m*n*epsilon_machine where epsilon_machine is
//          the relative machine precision.
//  t     - (optional) the total number of samples used for calculating
//          the covariance matrices.  Either  t = 0, or  t >= 2*(m+l)*s.
//          This parameter is not needed if the covariance matrices
//          and/or the Kalman predictor gain matrix are not desired.
//          If t = 0, then K, Q, Ry, and S are not computed.
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
    //  ALG
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


    return 0;
}