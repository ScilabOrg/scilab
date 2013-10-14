/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 *
 * (temporary function waiting for uicontextmenu function)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <stdio.h>
#include <string.h>
/*--------------------------------------------------------------------------*/
#include "gw_gui.h"
#include "api_scilab.h"
#include "localization.h"
#include "ContextMenu.h"
#include "Scierror.h"
#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
#include "warningmode.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int sci_mpopup(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iMenuitemIndex = 0;

    char *pstRes = NULL;
    int iUicontextmenuUID = 0;
    int iMenuitemUID = 0;
    char **pstAllMenuLabels = NULL;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (getWarningMode())
    {
        sciprint(_("%s: Feature %s is obsolete.\n"), _("Warning"), fname);
        sciprint(_("%s: Please use %s instead.\n"), _("Warning"), "uicontextmenu");
        sciprint(_("%s: This feature will be permanently removed in Scilab %s\n\n"), _("Warning"), "5.4.1");
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr))
    {
        iRet = getAllocatedMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &pstAllMenuLabels);
        if (iRet)
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstAllMenuLabels);
            return iRet;
        }
        if ((iRows != 1) && (iCols != 1))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string vector expected.\n"), fname, 1);
            freeAllocatedMatrixOfString(iRows, iCols, pstAllMenuLabels);
            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string vector expected.\n"), fname, 1);
        return FALSE;
    }

    /* Create an uicontextmenu */
    iUicontextmenuUID = createGraphicObject(__GO_UICONTEXTMENU__);
    if (iUicontextmenuUID != 0)
    {
        for (iMenuitemIndex = 0; iMenuitemIndex < iRows * iCols; iMenuitemIndex++)
        {
            // Create sub-menus
            iMenuitemUID = createGraphicObject(__GO_UIMENU__);
            setGraphicObjectProperty(iMenuitemUID, __GO_UI_LABEL__, pstAllMenuLabels[iMenuitemIndex], jni_string, 1);
            setGraphicObjectRelationship(iUicontextmenuUID, iMenuitemUID);
        }
    }

    pstRes = uiWaitContextMenu(iUicontextmenuUID);

    iRet = createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstRes);
    if (iRet)
    {
        freeAllocatedMatrixOfString(iRows, iCols, pstAllMenuLabels);
        return iRet;
    }

    freeAllocatedMatrixOfString(iRows, iCols, pstAllMenuLabels);

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    ReturnArguments(pvApiCtx);

    return TRUE;
}
/*--------------------------------------------------------------------------*/
