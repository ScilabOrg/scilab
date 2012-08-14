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

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 0, 1);



    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }//
    if (isStringType(pvApiCtx, &piAddr))
    {

        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &nbRowItems, &nbColItems, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piLen = (int*)malloc(sizeof(int) * nbRowItems * nbColItems);

        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &nbRowItems, &nbColItems, piLen, NULL);
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
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &nbRowItems, &nbColItems, piLen, itemsAdr);
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

    if (isStringType(pvApiCtx, TODO_ADDRESS_OF_2))
    {
        GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &messageAdr);
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
        if (isStringType(pvApiCtx, TODO_ADDRESS_OF_3))
        {
            GetRhsVar(3, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &buttonLabelAdr);
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

    nbRow = 1;
    nbCol = 1;
    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &userValueAdr);
    *stk(userValueAdr) = userValue;

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
