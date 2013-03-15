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

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "BOOL.h"
#include "MALLOC.h"
#include "gw_cacsd0.h"
/*--------------------------------------------------------------------------*/
#define CSIZE 64000

#define CHECK_PARAM(CTX, POS) \
    if(checkParam(CTX, POS)) return 0;
/*--------------------------------------------------------------------------*/
extern int C2F(dcopy)(int *, double *, int *, double *, int *);
extern int C2F(ib01ad)(char*, char*, char*, char*, char*, char*, int*, int*,
                       int*, int*, double*, int*, double*, int*, int*, double*, int*, double*, double*,
                       double*, int*, double*, int*, int*, int*);
extern int C2F(dlacpy) (char *_pstUplo, int *piM, int *_piN, double *_pdblA, int *_piLdA, double *_pdblB, int *_piLdB);

/*--------------------------------------------------------------------------*/
static int checkParam(void* _pvCtx, int _iPos)
{
    SciErr sciErr;
    int* piAddr = NULL;

    //get var address
    sciErr = getVarAddressFromPosition(_pvCtx, _iPos, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), "sorder", _iPos);
        return 1;
    }

    //check is real scalar double
    if ( isScalar(_pvCtx, piAddr) == 0 ||
            isDoubleType(_pvCtx, piAddr) == 0 ||
            isVarComplex(pvApiCtx, piAddr) == 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), "sorder", _iPos);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
static char getIntegerValue(void* _pvCtx, int _iPos)
{
    SciErr sciErr;
    int* piAddr = NULL;
    double dblVal;

    //get var address
    sciErr = getVarAddressFromPosition(_pvCtx, _iPos, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), "sorder", _iPos);
        return 1;
    }

    getScalarDouble(_pvCtx, piAddr, &dblVal);
    return (char)dblVal;
}
/*--------------------------------------------------------------------------*/
int sci_sorder(char* fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iOne = 1;

    char cMeth = 0;
    int iMeth = 0;
    char cAlg = 0;
    int iAlg = 0;
    char cJobd = 0;
    int iJobd = 0;
    char cBatch = 0;
    int iBatch = 0;
    char cConct = 0;
    int iConct = 0;
    char cCtrl = 0;
    int iNOBR = 0;

    int* piAddrY = NULL;
    int iSizeY = 0;
    int iNSMP = 0;
    int iL = 0;
    double* pdblY = NULL;

    int* piAddrU = NULL;
    int iSizeU = 0;
    int iRowsU = 0;
    int iM = 0;
    int iN = 0; //used but never filled
    double* pdblU = NULL;

    int iSizeR = 0;
    double* pdblR = NULL;

    double dblTol1 = 0;
    double dblTol2 = -1;

    BOOL bPrintw = FALSE;
    int iNCOL = 0;
    int iNRSAVE = 0;

    int iLDY = 0;
    int iLDU = 1;
    int iLDR = 0;
    int iLIWORK = 1;
    int iLDWORK = 0;
    int iLDWMIN = 0;

    int iRhs = nbInputArgument(pvApiCtx);
    int iLhs = nbOutputArgument(pvApiCtx);

    //some ubber explicit variables ...
    int iNR = 0;
    int iNSMPMN = 0;
    int iWARN = 0;
    int iINFO = 0;

    //working array
    double* pR = NULL;
    double* pSV = NULL;
    double* pU = NULL;
    double* pY = NULL;
    int* pIWORK = NULL;
    double* pDWORK = NULL;

    CheckInputArgumentAtLeast(pvApiCtx, 7);
    CheckOutputArgumentAtLeast(pvApiCtx, 1);


    //  METH
    CHECK_PARAM(pvApiCtx, 1);
    iMeth = getIntegerValue(pvApiCtx, 1);

    switch (iMeth)
    {
        case 1 :
            cMeth = 'M';
            break;
        case 2 :
            cMeth = 'N';
            break;
        default :
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "1", "2");
            return 0;
    }

    //  ALG
    CHECK_PARAM(pvApiCtx, 2);
    iAlg = getIntegerValue(pvApiCtx, 2);

    switch (iAlg)
    {
        case 1 :
            cAlg  = 'C';
            break;
        case 2 :
            cAlg = 'F';
            break;
        case 3 :
            cAlg = 'Q';
            break;
        default:
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 2, "1", "2", "3");
            return 0;
    }

    //  JOBD
    CHECK_PARAM(pvApiCtx, 3);
    iJobd = getIntegerValue(pvApiCtx, 3);

    switch (iJobd)
    {
        case 1 :
            cJobd = 'M';
            break;
        case 2 :
            cJobd = 'N';
            break;
        default :
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 3, "1", "2");
            return 0;

    }

    //  BATCH
    CHECK_PARAM(pvApiCtx, 4);
    iBatch = getIntegerValue(pvApiCtx, 4);

    switch (iBatch)
    {
        case 1 :
            cBatch  = 'F';
            break;
        case 2 :
            cBatch = 'I';
            break;
        case 3 :
            cBatch = 'L';
            break;
        case 4 :
            cBatch = 'O';
            break;
        default :
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), fname, 4, "1", "2", "3", "4");
            return 0;
    }


    //  CONCT
    CHECK_PARAM(pvApiCtx, 5);
    iConct = getIntegerValue(pvApiCtx, 5);

    switch (iConct)
    {
        case 1 :
            cConct = 'M';
            break;
        case 2 :
            cConct = 'N';
            break;
        default :
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 5, "1", "2");
            return 0;
    }

    //CTRL
    CHECK_PARAM(pvApiCtx, 6);
    iNOBR = getIntegerValue(pvApiCtx, 6);
    if (iNOBR < 1)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), fname, 6);
        return 0;
    }

    cCtrl = 'N';

    //  Y

    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddrY);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
        return 0;
    }

    getMatrixOfDouble(pvApiCtx, piAddrY, &iNSMP, &iL, &pdblY);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
        return 0;
    }

    iSizeY = iNSMP * iL;
    if (iL <= 0)
    {
        Scierror(999, _("%s: The system has no outputs\n"), fname);
        return 0;
    }

    //  U
    if (iRhs >= 8)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddrU);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 8);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddrU, &iRowsU, &iM, &pdblU);
        iSizeU = iRowsU * iM;
    }

    iNR = 2 * (iM + iL) * iNOBR;
    if (iBatch <= 2)
    {
        iNSMPMN = 2 * iNOBR;
    }
    else
    {
        iNSMPMN = iNR + 2 * iNOBR - 1;
    }

    if (iNSMP < iNSMPMN)
    {
        Scierror(999, _("%s: The number of samples should be at least %d\n"), fname, iNSMPMN);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddrY) == 0 || isVarComplex(pvApiCtx, piAddrY) == 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 7);
        return 0;
    }

    if (iM > 0)
    {
        if (isDoubleType(pvApiCtx, piAddrU) == 0 || isVarComplex(pvApiCtx, piAddrU) == 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 8);
            return 0;
        }

        if (iRowsU != iNSMP)
        {
            Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same row dimensions expected.\n"), fname, 7, 8);
            return 0;
        }
    }


    //  TOL
    if (iRhs >= 9)
    {
        int* piAddr = NULL;
        int iRows = 0;
        int iCols = 0;
        double* pdblTol = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
            return 0;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblTol);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
            return 0;
        }

        if (iRows * iCols > 0)
        {
            dblTol1 = pdblTol[0];
        }
        else if (iRows * iCols > 1)
        {
            dblTol2 = pdblTol[1];
        }
    }

    iNCOL = iNR;
    if (iBatch < 4)
    {
        if (iConct == 1)
        {
            iNCOL += 1;
        }

        if (iAlg == 2)
        {
            iNCOL += iM + iL + 1;
        }
    }

    iNRSAVE = (iNCOL - iNR ) * iNR;

    //  PRINTW
    if (iRhs >= 10)
    {
        int iPrintw = 0;
        CHECK_PARAM(pvApiCtx, 10);
        iPrintw = getIntegerValue(pvApiCtx, 10);

        switch (iPrintw)
        {
            case 1 :
                bPrintw = TRUE;
                break;
            case 0 :
                bPrintw = FALSE;
                break;
            default :
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 10, "0", "1");
                return 0;
        }
    }

    //  Determine the lengths of working arrays.
    //  The quasi-optimal value for LDWORK (assumed block-size 64) is possibly
    //  modified, so that the sum of the lengths of DWORK and other needed
    //  arrays does not exceed the cache size. Specifically, the default value
    //  for LDWORK is computed using the formulas
    //            nr = 2*( m + l )*s
    //            LDWORK = ( t - 2*s + 3 + 64 )*nr
    //            if ( CSIZE > MAX( nr*nr + t*( m + l ) + 16, 2*nr ) ) then
    //               LDWORK = MIN( LDWORK, CSIZE - nr*nr - t*( m + l ) - 16 )
    //            else
    //               LDWORK = MIN( LDWORK, MAX( 2*nr, CSIZE/2 ) )
    //            end if
    //            LDWORK = MAX( minimum workspace size needed, LDWORK )
    //  where CSIZE is the cache size in double precision words.
    //  If LDWORK is specified,
    //         but it is less than the minimum workspace size
    //  needed, that minimum value is used instead.

    iLDY = iNSMP;
    if (iM > 0)
    {
        iLDU = iLDY;
    }

    iLDR = iNR;
    if (iMeth == 1 && iJobd == 1)
    {
        iLDR = Max(iNR, 3 * iM * iNOBR);
    }

    if (iMeth == 2)
    {
        iLIWORK = (iM + iL) * iNOBR;
    }
    else if (iAlg == 2)
    {
        iLIWORK = iM + iL;
    }


    //  The next statement is included in order to allow switching
    //  from Cholesky or fast QR to QR algorithm.

    iLDWORK = (iNSMP - 2 * iNOBR + 3 + 64) * iNR;
    if (Max(iNR * iNR + iNSMP * (iM + iL ) + 16, 2 * iNR) < CSIZE)
    {
        iLDWORK = Min(iLDWORK, CSIZE - iNR * iNR - iNSMP * (iM + iL) - 16);
    }
    else
    {
        iLDWORK = Min(iLDWORK , Max(2 * iNR, CSIZE / 2));
    }

    iLDWMIN = 2 * iNR * (iNOBR + 1);
    if (iAlg == 2)
    {
        iLDWMIN = Max(iLDWMIN, Max(2 * iNR * (iM + iL + 1) + iNR, iNR * (iM + iL + 3)));
    }

    iLDWORK = Max(iLDWORK, iLDWMIN);

    //  LDWORK
    if (iRhs >= 11)
    {
        int iTmp = 0;
        CHECK_PARAM(pvApiCtx, 11);
        getIntegerValue(pvApiCtx, 11);
        if (iTmp < iLDWMIN)
        {
            iLDWORK = iLDWMIN;
        }
        else
        {
            iLDWORK = iTmp;
        }

        //  R(2*(m+p)*s,2*(m+p)*s(+c)), where
        //  c = 1,       if conct = 1 and batch < 4;
        //  c = 0,       if conct = 2 or  batch = 4;
        //  c = c+m+l+1, if  alg  = 2 and batch < 4.

        if (iRhs >= 12 && (iBatch == 2 || iBatch == 3))
        {
            int* piAddr = NULL;
            int iRows = 0;
            int iCols = 0;

            sciErr = getVarAddressFromPosition(pvApiCtx, 12, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
                return 0;
            }

            getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblR);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 12);
                return 0;
            }

            if (iRows != iNR || iCols != iNCOL)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %dx%d expected.\n"), fname, 12, iNR, iNCOL);
                return 0;
            }

            iSizeR = iRows * iCols;
        }
    }

    //  Allocate variable dimension local arrays.
    pR = (double*)MALLOC(iLDR * iNCOL * sizeof(double));
    pSV = (double*)MALLOC(iLDR * iNCOL * sizeof(double));
    pU = (double*)MALLOC(iLDR * iNCOL * sizeof(double));
    pY = (double*)MALLOC(iLDR * iNCOL * sizeof(double));
    pIWORK = (int*)MALLOC(iLDR * iNCOL * sizeof(int));
    pDWORK = (double*)MALLOC(iLDR * iNCOL * sizeof(double));

    memset(pR, 0x00, iLDR * iNCOL * sizeof(double));

    // Copy inputs from scilab workspace to locally allocated arrays.
    C2F(dcopy)(&iSizeY, pdblY, &iOne, pY, &iOne);

    if (iM > 0)
    {
        C2F(dcopy)(&iSizeU, pdblU, &iOne, pU, &iOne);
    }

    if (iRhs >= 12 && (iBatch == 2 || iBatch == 3))
    {
        C2F(dcopy)(&iSizeR, pdblR, &iOne, pR, &iOne);
    }

    if (iAlg == 2 || iConct == 1)
    {
        if (iLDR == iNR)
        {
            C2F(dcopy)(&iNRSAVE, pR + iNR * iLDR, &iOne, pDWORK, &iOne);
        }
        else
        {
            C2F(dcopy)(&iSizeR, pR + iNR * iNR, &iOne, pDWORK, &iOne);
        }
    }

    // Do the actual computations.

    C2F(ib01ad)(&cMeth, &cAlg , &cJobd, &cBatch, &cConct, &cCtrl, &iNOBR, &iM, &iL,
                &iNSMP, pU, &iLDU, pY, &iLDY, &iN, pR, &iLDR, pSV, &dblTol1,
                &dblTol2, pIWORK, pDWORK, &iLDWORK, &iWARN, &iINFO);

    if (iWARN != 0 && bPrintw)
    {
        sciprint("IWARN = %d on exit from IB01AD\n", iWARN);
    }

    if (iINFO != 0)
    {
        sciprint("INFO = %d on exit from IB01AD\n", iINFO);
    }

    //  Copy output to scilab workspace.
    if (iLDR > iNR)
    {
        C2F(dlacpy)("FULL", &iNR, &iNR, pR, &iLDR, pR, &iNR);
    }

    if ((iAlg == 2 || iConct == 1) && iBatch <= 2)
    {
        if (iLDR == iNR)
        {
            C2F(dcopy)(&iNRSAVE, pDWORK, &iOne, pR + iNR * iLDR, &iOne);
        }
        else
        {
            C2F(dcopy)(&iNRSAVE, pDWORK, &iOne, pR + iNR * iNR, &iOne);
        }
    }

    createMatrixOfDouble(pvApiCtx, iRhs + 1, iNR, iNCOL, pR);

    if (iLhs > 1 && iBatch > 2)
    {
        createScalarDouble(pvApiCtx, iRhs + 2, (double)iN);
    }
    else if (iLhs > 1)
    {
        createEmptyMatrix(pvApiCtx, iRhs + 2);
    }

    if (iLhs > 2 && iBatch > 2)
    {
        createMatrixOfDouble(pvApiCtx, iRhs + 3, iL * iNOBR, 1, pSV);
    }
    else if (iLhs > 2)
    {
        createEmptyMatrix(pvApiCtx, iRhs + 3);
    }

    if (iLhs > 3 && iBatch > 2)
    {
        createMatrixOfDouble(pvApiCtx, iRhs + 4, 2, 1, pDWORK + 1);
    }
    else if (iLhs > 3)
    {
        createEmptyMatrix(pvApiCtx, iRhs + 4);
    }


    if (iRhs > 3)
    {
        AssignOutputVariable(pvApiCtx, 4) = iRhs + 4;
    }

    if (iRhs > 3)
    {
        AssignOutputVariable(pvApiCtx, 3) = iRhs + 3;
    }

    if (iRhs > 3)
    {
        AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;


    ReturnArguments(pvApiCtx);
    return 0;
}
