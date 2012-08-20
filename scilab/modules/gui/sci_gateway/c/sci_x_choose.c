/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "api_scilab.h"
#include "gw_gui.h"
#include "stack-c.h"
#include "localization.h"
#include "CallMessageBox.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
/*--------------------------------------------------------------------------*/
int sci_x_choose(char *fname, unsigned long fname_len)
{
    int nbRow = 0, nbCol = 0;
    int nbRowItems = 0, nbColItems = 0;

    int messageBoxID = 0;

    char **itemsAdr = NULL;
    char **buttonLabelAdr = NULL;

    char **messageAdr = NULL;

    int userValueAdr = 0;
    int userValue = 0;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piLen		= NULL;

    int i;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 0, 1);



    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }//
    if (isStringType(pvApiCtx, piAddr1))
    {

        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr1, &nbRowItems, &nbColItems, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piLen = (int*)malloc(sizeof(int) * nbRowItems * nbColItems);

        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr1, &nbRowItems, &nbColItems, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        itemsAdr = (char**)malloc(sizeof(char*) * nbRowItems * nbColItems);
        for (i = 0 ; i < nbRowItems * nbColItems ; i++)
        {
            itemsAdr[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        //third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr1, &nbRowItems, &nbColItems, piLen, itemsAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname);
        return FALSE;
    }



    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }//
    if (isStringType(pvApiCtx, piAddr2))
    {

        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr2, &nbRow, &nbCol, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piLen = (int*)malloc(sizeof(int) * nbRow * nbCol);

        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr2, &nbRow, &nbCol, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        messageAdr = (char**)malloc(sizeof(char*) * nbRow * nbCol);
        for (i = 0 ; i < nbRow * nbCol ; i++)
        {
            messageAdr[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        //third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr2, &nbRow, &nbCol, piLen, messageAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
        return FALSE;
    }

    /* Create the Java Object */
    messageBoxID = createMessageBox();

    /* Title is a default title */
    setMessageBoxTitle(messageBoxID, _("Scilab Choose Message"));
    /* Message */
    setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack((size_t)messageAdr), nbCol * nbRow);
    /* ListBox Items */
    setMessageBoxListBoxItems(messageBoxID, getStringMatrixFromStack((size_t)itemsAdr), nbColItems * nbRowItems);
    /* Modality */
    setMessageBoxModal(messageBoxID, TRUE);

    if (Rhs == 3)
    {


        //get variable address
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }//
        if (isStringType(pvApiCtx, piAddr3))
        {

            //fisrt call to retrieve dimensions
            sciErr = getMatrixOfString(pvApiCtx, piAddr3, &nbRow, &nbCol, NULL, NULL);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            piLen = (int*)malloc(sizeof(int) * nbRow * nbCol);

            //second call to retrieve length of each string
            sciErr = getMatrixOfString(pvApiCtx, piAddr3, &nbRow, &nbCol, piLen, NULL);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            buttonLabelAdr = (char**)malloc(sizeof(char*) * nbRow * nbCol);
            for (i = 0 ; i < nbRow * nbCol ; i++)
            {
                buttonLabelAdr[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
            }

            //third call to retrieve data
            sciErr = getMatrixOfString(pvApiCtx, piAddr3, &nbRow, &nbCol, piLen, buttonLabelAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            if (nbRow*nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
            return FALSE;
        }

        setMessageBoxButtonsLabels(messageBoxID, getStringMatrixFromStack((size_t)buttonLabelAdr), nbCol * nbRow);
    }

    /* Display it and wait for a user input */
    messageBoxDisplayAndWait(messageBoxID);

    /* Read the user answer */
    userValue = getMessageBoxSelectedItem(messageBoxID);

    /* Create the matrix as return of the function */
    createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, userValue);


    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
