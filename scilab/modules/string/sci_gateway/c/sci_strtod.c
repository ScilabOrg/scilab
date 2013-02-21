/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include "gw_string.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
/*----------------------------------------------------------------------------*/
int sci_strtod(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iType = 0;
    int iRows = 0, iCols = 0;
    char **Input_StringMatrix_1 = NULL;
    int iRowsiCols = 0;
    int *pilen = NULL;
    int first_nb = 0;
    int i, x, y; //loop indexes
    char keys[] = "1234567890";
    unsigned long long raw = 0x7ff8000000000000;
    double not_a_number = *( double* )&raw;
    int flag = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 2);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr)) //Check type
    {

        //First call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        pilen = (int*)malloc(sizeof(int) * iRows * iCols);

        //Second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, pilen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        Input_StringMatrix_1 = (char**)malloc(sizeof(char*) * iRows * iCols);
        iRowsiCols = iRows * iCols ;
        for (i = 0; i < iRowsiCols; i++)
        {
            Input_StringMatrix_1[i] = (char*)malloc(sizeof(char) * (pilen[i] + 1));
        }

        //Third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, pilen , Input_StringMatrix_1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (nbOutputArgument(pvApiCtx) == 2)
        {
            double *OutputDoubles = NULL;
            char **OutputStrings = NULL;

            OutputStrings = (char **)MALLOC(sizeof(char*)*iRowsiCols);
            if (OutputStrings == NULL)
            {
                freeAllocatedMatrixOfString(iRows, iCols, Input_StringMatrix_1);
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            OutputDoubles = (double*)MALLOC(sizeof(double) * iRowsiCols);
            if (OutputDoubles == NULL)
            {
                FREE(OutputStrings);
                OutputStrings = NULL;
                freeAllocatedMatrixOfString(iRows, iCols, Input_StringMatrix_1);
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            for ( x = 0 ; x < iRowsiCols ; x++ )
            {
                //Double part
                char *stopstring = NULL;
                first_nb = (int)strcspn(Input_StringMatrix_1[x], keys);

                //Check if there is a number in the string
                if (first_nb != 0)
                {
                    flag = 0;
                    for ( y = 0; y < first_nb; y++)
                    {
                        if (Input_StringMatrix_1[x][y] != ' ')
                        {
                            OutputDoubles[x] = not_a_number;
                            flag = 1;
                            stopstring = Input_StringMatrix_1[x];
                        }
                    }
                    if (flag == 0)
                    {
                        OutputDoubles[x] = (double)strtod(Input_StringMatrix_1[x], &stopstring);
                    }
                }
                else
                {
                    OutputDoubles[x] = (double)strtod(Input_StringMatrix_1[x], &stopstring);
                }

                //String part
                if (stopstring)
                {
                    OutputStrings[x] = (char*)MALLOC(sizeof(char) * (strlen(stopstring) + 1));
                }
                else
                {
                    OutputStrings[x] = (char*)MALLOC(sizeof(char) * (strlen("") + 1));
                }

                if (OutputStrings[x] == NULL)
                {
                    freeAllocatedMatrixOfString(iRows, iCols, Input_StringMatrix_1);
                    freeAllocatedMatrixOfString(iRows, iCols, OutputStrings);
                    if (OutputDoubles)
                    {
                        FREE(OutputDoubles);
                        OutputDoubles = NULL;
                    }
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;

                }

                if (stopstring)
                {
                    strcpy(OutputStrings[x], stopstring);
                }
                else
                {
                    strcpy(OutputStrings[x], "");
                }
            }

            //CreateVarFromPtr(nbInputArgument(pvApiCtx)+1,MATRIX_OF_DOUBLE_DATATYPE,&iRows,&iCols,&OutputDoubles);
            createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, OutputDoubles);
            //CreateVarFromPtr(nbInputArgument(pvApiCtx)+2,MATRIX_OF_STRING_DATATYPE,&iRows,&iCols,OutputStrings);
            createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 2, iRows, iCols, OutputStrings);

            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1 ;
            AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2 ;

            freeAllocatedMatrixOfString(iRows, iCols, OutputStrings);
            if (OutputDoubles)
            {
                FREE(OutputDoubles)
            }
        }
        else /* nbOutputArgument(pvApiCtx) == 1 */
        {
            //int outIndex = 0;
            //CreateVar(nbInputArgument(pvApiCtx)+1,MATRIX_OF_DOUBLE_DATATYPE,&iRows,&iCols,&outIndex);
            double* pdblOutIndex = NULL;
            allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, &pdblOutIndex);
            for ( x = 0 ; x < iRowsiCols ; x++ )
            {
                char  *stopstring = NULL;
                first_nb = (int)strcspn(Input_StringMatrix_1[x], keys);

                //Check if there is a number in the string
                if (first_nb != 0)
                {
                    flag = 0;
                    for ( y = 0; y < first_nb; y++)
                    {
                        if (Input_StringMatrix_1[x][y] != ' ')
                        {
                            pdblOutIndex[x] = not_a_number;
                            flag = 1;
                        }
                    }

                    if (flag == 0)
                    {
                        pdblOutIndex[x] = (double)strtod( Input_StringMatrix_1[x], &stopstring);
                    }
                }
                else
                {
                    pdblOutIndex[x] = (double)strtod( Input_StringMatrix_1[x], &stopstring);
                }

            }
            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1 ;
        }

        freeAllocatedMatrixOfString(iRows, iCols, Input_StringMatrix_1);
        ReturnArguments(pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname, 1);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/

