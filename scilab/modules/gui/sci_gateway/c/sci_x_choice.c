/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java implementation)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "localization.h"
#include "CallMessageBox.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "freeArrayOfString.h"

/*--------------------------------------------------------------------------*/
int sci_x_choice(char *fname, unsigned long fname_len)
{
    int nbRow = 0, nbCol = 0;
    int nbRowDefaultValues = 0, nbColDefaultValues = 0;
    int nbRowLineLabels = 0, nbColLineLabels = 0;

    int messageBoxID = 0;

    char **labelsAdr = NULL;
    char **lineLabelsAdr = NULL;
    int defaultValuesAdr = 0;
    double *defaultValues = NULL;
    int *defaultValuesInt = NULL;

    int userValueSize = 0;
    int *userValue = NULL;
    double *userValueDouble = NULL;

    int emptyMatrixAdr = 0;

    int K = 0;

    nbInputArgument(pvApiCtx, 3, 3);
    nbOutputArgument(pvApiCtx, 0, 1);

    /* READ THE DEFAULT VALUES */
if (isDoubleType(pvApiCtx, TODO: ADDRESS_OF_1)
{
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRowDefaultValues, &nbColDefaultValues, &defaultValuesAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        defaultValues = getDoubleMatrixFromStack(defaultValuesAdr);

        defaultValuesInt = (int *)MALLOC(nbRowDefaultValues * nbColDefaultValues * sizeof(int));
        for (K = 0; K < nbRowDefaultValues * nbColDefaultValues; K++)
        {
            defaultValuesInt[K] = (int)defaultValues[K];
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"), fname, 1);
        return FALSE;
    }

    /* READ THE MESSAGE */
if (isStringType(pvApiCtx, TODO: ADDRESS_OF_2)
{

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &nbRow, &nbCol, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piLen = (int*)malloc(sizeof(int) * nbRow * nbCol);

        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &nbRow, &nbCol, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        labelsAdr = (char**)malloc(sizeof(char*) * nbRow * nbCol);
        for (i = 0 ; i < nbRow * nbCol ; i++)
        {
            labelsAdr[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        //third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &nbRow, &nbCol, piLen, labelsAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
        FREE(defaultValuesInt);
        return FALSE;
    }

    /* Create the Java Object */
    messageBoxID = createMessageBox();

    /* Title is a default title */
    setMessageBoxTitle(messageBoxID, _("Scilab Choices Request"));

    /* Message */
    setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack((size_t)labelsAdr), nbCol * nbRow);
    freeArrayOfString(labelsAdr, nbRow * nbCol);

    /* READ THE LABELS */
if (isStringType(pvApiCtx, TODO: ADDRESS_OF_3)
{
    GetRhsVar(3, MATRIX_OF_STRING_DATATYPE, &nbRowLineLabels, &nbColLineLabels, &lineLabelsAdr);
        if (nbRow != 1 && nbCol != 1)
        {
            freeArrayOfString(lineLabelsAdr, nbRowLineLabels * nbColLineLabels);
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector of strings expected.\n"), fname, 3);
            return FALSE;
        }
        setMessageBoxLineLabels(messageBoxID, getStringMatrixFromStack((size_t)lineLabelsAdr), nbColLineLabels * nbRowLineLabels);
        freeArrayOfString(lineLabelsAdr, nbRowLineLabels * nbColLineLabels);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 3);
        return FALSE;
    }

    /* Default selected buttons */
    setMessageBoxDefaultSelectedButtons(messageBoxID, defaultValuesInt, nbRowDefaultValues * nbColDefaultValues);

    /* Display it and wait for a user input */
    messageBoxDisplayAndWait(messageBoxID);

    /* Read the user answer */
    userValueSize = getMessageBoxValueSize(messageBoxID);
    if (userValueSize == 0)
{
    nbRow = 0;
    nbCol = 0;
    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &emptyMatrixAdr);
    }
    else
    {

        userValue = (int*)getMessageBoxUserSelectedButtons(messageBoxID);

        userValueDouble = (double *)MALLOC(nbRowDefaultValues * nbColDefaultValues * sizeof(double));
        for (K = 0; K < nbRowDefaultValues * nbColDefaultValues; K++)
        {
            userValueDouble[K] = userValue[K];
        }

        /* Create the matrix as return of the function */
        sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRowDefaultValues, nbColDefaultValues, userValueDouble);
        free(userValueDouble); // Data have been copied into Scilab memory
        if (sciErr.iErr)
        {
            free(userValueDouble); // Make sure everything is cleanup in case of error
            printError(&sciErr, 0);
            return 0;
            /* TO DO : do a delete []  getMessageBoxUserSelectedButton */
        }

        FREE(defaultValuesInt);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments();
        return TRUE;
    }
    /*--------------------------------------------------------------------------*/
