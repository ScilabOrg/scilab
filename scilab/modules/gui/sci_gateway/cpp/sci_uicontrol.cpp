/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C" {
#include "gw_gui.h"
/*--------------------------------------------------------------------------*/
#include "CreateUIControl.h"
#include "HandleManagement.h"
#include "MALLOC.h"             /* MALLOC */
#include "localization.h"
#include "stricmp.h"
#include "stack-c.h"
#include "SetPropertyStatus.h"
#include "SetHashTable.h"
#include "Scierror.h"
#include "FigureList.h"         /* getFigureFromIndex */
#include "Widget.h"             /* requestWidgetFocus */
#include "freeArrayOfString.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "createGraphicObject.h"
#include "UIControl.h"
}

#include <vector>
#include "CallScilabBridge.hxx"
#include "ScilabToJava.hxx"

/*--------------------------------------------------------------------------*/
int sci_uicontrol(char *fname, unsigned long fname_len)
{
    static int handlerId = org_scilab_modules_gui_bridge::CallScilabBridge::initUIControl(getScilabJavaVM());

    std::vector<int> indexes;
    SciErr sciErr;
    int start = 1;
    int propertyIndex = 0;
    int propertiesCount = 0;
    int parentUID = 0;
    int uicontrolUID = 0;
    long GraphicHandle = -1;
    char ** pnames = NULL;
    int userDataPos = -1;

    CheckOutputArgument(pvApiCtx, 0, 1);

    if ((nbInputArgument(pvApiCtx) % 2) == 1)
    {
        if (!checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            OverLoad(1);
            return FALSE;
        }
        else
        {
            int* piAddr = NULL;
            long long hParent = 0;

            propertiesCount = (nbInputArgument(pvApiCtx) - 1) / 2;
            start = 2;

            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return FALSE;
            }

            if (isScalar(pvApiCtx, piAddr) == 0)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return FALSE;
            }

            if (getScalarHandle(pvApiCtx, piAddr, &hParent))
            {
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                return FALSE;
            }

            parentUID = getObjectFromHandle((long)hParent);
        }
    }
    else
    {
	if (!checkInputArgumentType(pvApiCtx, 1, sci_handles) && !checkInputArgumentType(pvApiCtx, 1, sci_strings))
        {
            OverLoad(1);
            return FALSE;
        }
        propertiesCount = nbInputArgument(pvApiCtx) / 2;
    }

    if (propertiesCount > 0)
    {
        pnames = (char**)MALLOC(sizeof(char *) * propertiesCount);
        if (!pnames)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return FALSE;
        }

        for (int i = 0; i < propertiesCount; i++)
        {
            int * piAddr = NULL;
            char * propertyName = NULL;

            sciErr = getVarAddressFromPosition(pvApiCtx, 2 * i + start, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return FALSE;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddr, &propertyName))
            {
		freeAllocatedMatrixOfString(1, propertyIndex, pnames);
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2 * i + start);
                return FALSE;
            }

            if (stricmp(propertyName, "parent") == 0)
            {
                long long hParent = 0;
                freeAllocatedSingleString(propertyName);

                sciErr = getVarAddressFromPosition(pvApiCtx, 2 * i + start + 1, &piAddr);
                if (sciErr.iErr)
                {
                    freeAllocatedMatrixOfString(1, propertyIndex, pnames);
                    printError(&sciErr, 0);
                    return 1;
                }

                if (isScalar(pvApiCtx, piAddr) == 0)
                {
                    freeAllocatedMatrixOfString(1, propertyIndex, pnames);
                    Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                    return FALSE;
                }

                if (getScalarHandle(pvApiCtx, piAddr, &hParent))
                {
                    freeAllocatedMatrixOfString(1, propertyIndex, pnames);
                    Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                    return FALSE;
                }

                parentUID = getObjectFromHandle((long) hParent);
            }
            else if (stricmp(propertyName, "user_data") == 0 || stricmp(propertyName, "userdata") == 0)
            {
                userDataPos = 2 * i + start + 1;
            }
            else
            {
                pnames[propertyIndex++] = propertyName;
                sciErr = getVarAddressFromPosition(pvApiCtx, 2 * i + start + 1, &piAddr);
                if (sciErr.iErr)
                {
                    freeAllocatedMatrixOfString(1, propertyIndex, pnames);
                    printError(&sciErr, 0);
                    return FALSE;
                }
                org_modules_types::ScilabToJava::sendVariable("", piAddr, false, handlerId, pvApiCtx);
            }
        }

        org_modules_types::ScilabToJava::sendStringVariable("", indexes, propertyIndex, 1, pnames, false, false, handlerId);
        freeAllocatedMatrixOfString(1, propertyIndex, pnames);
    }

    if (parentUID == 0)
    {
        parentUID = getCurrentFigure();
        if (parentUID == 0)
        {
            parentUID = createNewFigureWithAxes();
        }
    }

    if (nbInputArgument(pvApiCtx) == 1)
    {
        int type = 0;
        int * _type = &type;

        getGraphicObjectProperty(parentUID, __GO_TYPE__, jni_int, (void **)&_type);
        if (type == __GO_UICONTROL__)
        {
            requestFocus(parentUID);
            GraphicHandle = getHandle(parentUID);
        }
        if (type != __GO_FIGURE__ && type != __GO_UIMENU__)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s', '%s' or '%s' handle expected.\n"), fname, 1, "Uicontrol", "Figure", "Uimenu");
	    return FALSE;
        }
    }

    if (GraphicHandle == -1)
    {
        uicontrolUID = createGraphicObject(__GO_UICONTROL__);
        setGraphicObjectRelationship(parentUID, uicontrolUID);
        GraphicHandle = getHandle(uicontrolUID);

        if (userDataPos != -1)
        {
            setUserData(uicontrolUID, userDataPos, pvApiCtx);
        }
    }

    /* Create return variable */
    if (createScalarHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, GraphicHandle))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return FALSE;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}

/*--------------------------------------------------------------------------*/
