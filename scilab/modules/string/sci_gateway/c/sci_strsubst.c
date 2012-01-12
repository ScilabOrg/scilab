/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU , Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"
#include "strsubst.h"
#include "localization.h"
#include "Scierror.h"
#include "pcre_error.h"
#include "pcre_private.h"
#include "BOOL.h"
/*-------------------------------------------------------------------------------------*/
#define CHAR_R "r"
#define CHAR_S "s"
/*-------------------------------------------------------------------------------------*/
int sci_strsubst(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int iType = 0;
    int mOne = 0, nOne = 0;

    CheckRhs(3, 4);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }
    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType == sci_strings)
    {
        char **pStVarOne = NULL;
        int *lenStVarOne = NULL;

        int *piAddressVarTwo = NULL;
        int iType2 = 0;
        int mTwo = 0, nTwo = 0;
        char *pStVarTwo = NULL;
        int lenStVarTwo = 0;

        int *piAddressVarThree = NULL;
        int mThree = 0, nThree = 0;
        char *pStVarThree = NULL;
        int lenStVarThree = 0;
        int iType3 = 0;

        BOOL isRegExp = FALSE;

        if (Rhs == 4)
        {
            int *piAddressVarFour = NULL;
            int iType4 = 0;
            int mFour = 0, nFour = 0;
            char *pStVarFour = NULL;
            int lenStVarFour = 0;

            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
                return 0;
            }

            sciErr = getVarType(pvApiCtx, piAddressVarFour, &iType4);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
                return 0;
            }

            if (iType4 != sci_strings)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 4);
                return 0;
            }

            sciErr = getMatrixOfString(pvApiCtx, piAddressVarFour, &mFour, &nFour, &lenStVarFour, NULL);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
                return 0;
            }

            if ((mFour != nFour) && (nFour != 1))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 4);
                return 0;
            }

            pStVarFour = (char *)MALLOC(sizeof(char) * (lenStVarFour + 1));
            if (pStVarFour)
            {
                sciErr = getMatrixOfString(pvApiCtx, piAddressVarFour, &mFour, &nFour, &lenStVarFour, &pStVarFour);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
                    return 0;
                }

                if ((strcmp(pStVarFour, CHAR_R) == 0) || (strcmp(pStVarFour, CHAR_S) == 0))
                {
                    if (strcmp(pStVarFour, CHAR_R) == 0)
                    {
                        isRegExp = TRUE;
                    }
                    else
                    {
                        isRegExp = FALSE;
                    }
                    FREE(pStVarFour);
                    pStVarFour = NULL;
                }
                else
                {
                    FREE(pStVarFour);
                    pStVarFour = NULL;
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 4, "s", "r");
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        // checks type 3th input argument
        sciErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (iType3 != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
            return 0;
        }

        // checks dimension 3th input argument
        sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &mThree, &nThree, &lenStVarThree, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if ((mThree != nThree) && (nThree != 1))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        // checks type 2nd input argument
        sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (iType2 != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        // checks dimension 2nd input argument
        sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &mTwo, &nTwo, &lenStVarTwo, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if ((mTwo != nTwo) && (nTwo != 1))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        // checks type 1st input argument
        sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
        if (iType != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 0;
        }

        // get length 3th input argument
        sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &mThree, &nThree, &lenStVarThree, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        pStVarThree = (char *)MALLOC(sizeof(char) * (lenStVarThree + 1));
        if (pStVarThree)
        {
            // get value 3th input argument
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &mThree, &nThree, &lenStVarThree, &pStVarThree);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }

            // get length 2nd input argument
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &mTwo, &nTwo, &lenStVarTwo, NULL);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }

            pStVarTwo = (char *)MALLOC(sizeof(char) * (lenStVarTwo + 1));
            if (pStVarTwo)
            {
                // get value 2nd input argument
                sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &mTwo, &nTwo, &lenStVarTwo, &pStVarTwo);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                    return 0;
                }

                sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, lenStVarOne, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 0;
                }

                lenStVarOne = (int *)MALLOC(sizeof(int) * (mOne * nOne));
                if (lenStVarOne)
                {
                    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, lenStVarOne, NULL);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                        return 0;
                    }

                    pStVarOne = (char **)MALLOC(sizeof(char *) * (mOne * nOne));
                    if (pStVarOne)
                    {
                        char **Output_StringMatrix = NULL;

                        int i = 0;

                        for (i = 0; i < mOne * nOne; i++)
                        {
                            pStVarOne[i] = (char *)MALLOC(sizeof(char) * (lenStVarOne[i] + 1));
                            if (pStVarOne[i] == NULL)
                            {
                                FREE(pStVarThree);
                                pStVarThree = NULL;
                                FREE(pStVarTwo);
                                pStVarTwo = NULL;
                                FREE(lenStVarOne);
                                lenStVarOne = NULL;
                                freeArrayOfString(pStVarOne, i);
                                Scierror(999, _("%s: No more memory.\n"), fname);
                                return 0;
                            }
                        }

                        sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, lenStVarOne, pStVarOne);
                        if (sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                            return 0;
                        }

                        FREE(lenStVarOne);
                        lenStVarOne = NULL;

                        if (isRegExp)
                        {
                            int ierr = (int)PCRE_FINISHED_OK;

                            Output_StringMatrix = strsubst_reg(pStVarOne, mOne * nOne, pStVarTwo, pStVarThree, &ierr);
                            if ((ierr != PCRE_FINISHED_OK) && (ierr != NO_MATCH) && (ierr != PCRE_EXIT))
                            {
                                FREE(pStVarThree);
                                pStVarThree = NULL;
                                FREE(pStVarTwo);
                                pStVarTwo = NULL;
                                pcre_error(fname, ierr);
                                return 0;
                            }
                        }
                        else
                        {
                            Output_StringMatrix = strsubst(pStVarOne, mOne * nOne, pStVarTwo, pStVarThree);
                        }

                        FREE(pStVarThree);
                        pStVarThree = NULL;
                        FREE(pStVarTwo);
                        pStVarTwo = NULL;
                        freeArrayOfString(pStVarOne, mOne * nOne);

                        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, mOne, nOne, Output_StringMatrix);
                        freeArrayOfString(Output_StringMatrix, mOne * nOne);
                        if (sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            Scierror(999,_("%s: Memory allocation error.\n"), fname);
                            return 0;
                        }

                        LhsVar(1) = Rhs + 1;
                        PutLhsVar();
                        return 0;
                    }
                    else
                    {
                        FREE(pStVarThree);
                        pStVarThree = NULL;
                        FREE(pStVarTwo);
                        pStVarTwo = NULL;
                        FREE(lenStVarOne);
                        lenStVarOne = NULL;
                        Scierror(999, _("%s: No more memory.\n"), fname);
                        return 0;
                    }
                }
                else
                {
                    FREE(pStVarThree);
                    pStVarThree = NULL;
                    FREE(pStVarTwo);
                    pStVarTwo = NULL;
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }
            }
            else
            {
                FREE(pStVarThree);
                pStVarThree = NULL;
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }
    else if (iType == sci_matrix)
    {
        sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &mOne, &nOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if ((mOne == 0) && (nOne == 0)) /* strsubst([],...) returns [] */
        {
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, mOne, nOne, NULL);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999,_("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
    }
    return 0;
}

/*-------------------------------------------------------------------------------------*/
