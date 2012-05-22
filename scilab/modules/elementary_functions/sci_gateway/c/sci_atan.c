/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "msgs.h"

/*--------------------------------------------------------------------------*/
int sci_atan(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int i;
    int iRows1						= 0;
    int iCols1						= 0;
    int iType1						= 0;
    int iRows2						= 0;
    int iCols2						= 0;
    int iType2						= 0;

    int* piAddr1					= NULL;
    int* piAddr2					= NULL;

    double *pdblReal1			= NULL;
    double *pdblImg1			= NULL;
    double *pdblReal2			= NULL;
    double *pdblImg2			= NULL;
    double *pdblRealRet		= NULL;
    double *pdblImgRet		= NULL;

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iType1 != sci_matrix)
    {
        OverLoad(1);
        return 0;
    }

    if (Rhs == 1)
    {
        if (isVarComplex(pvApiCtx, piAddr1))
        {
            // case complex
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr1, &iRows1, &iCols1, &pdblReal1, &pdblImg1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iRows1, iCols1, &pdblRealRet, &pdblImgRet);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            for (i = 0 ; i < iRows1 * iCols1 ; i++)
            {
                if (pdblReal1[i] == 0 && dabss(pdblImg1[i]) == 1)
                {
                    if (C2F(errgst).ieee == 0)
                        SciError(32);
                    else if (C2F(errgst).ieee == 1)
                        Msgs(64, 0);
                }
                watan(pdblReal1[i], pdblImg1[i], &pdblRealRet[i], &pdblImgRet[i]);
            }
        }
        else
        {
            // case real
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &iRows1, &iCols1, &pdblReal1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows1, iCols1, &pdblRealRet);
            for (i = 0 ; i < iRows1 * iCols1 ; i++)
            {
                pdblRealRet[i] = datans(pdblReal1[i]);
            }
        }
    }
    else
    {
        //Rhs == 2
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (isVarComplex(pvApiCtx, piAddr1) == FALSE && isVarComplex(pvApiCtx, piAddr2) == FALSE)
        {
            //Only works with real matrix
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &iRows1, &iCols1, &pdblReal1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows2, &iCols2, &pdblReal2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            if (iRows1 * iCols1 == iRows2 * iCols2)
            {
                sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows1, iCols1, &pdblRealRet);
                for (i = 0 ; i < iRows1 * iCols1 ; i++)
                {
                    pdblRealRet[i] = datan2s(pdblReal1[i], pdblReal2[i]);
                }
            }
            else
                SciError(60);
        }
        else
            SciError(43);
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
