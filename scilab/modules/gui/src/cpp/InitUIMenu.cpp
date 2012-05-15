/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - Yung-Jang Lee
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "InitUIMenu.hxx"
#include "CallScilabBridge.hxx"
extern "C"
{
#include "sci_types.h"
#include "BOOL.h"
#include "getScilabJavaVM.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "getPropertyAssignedValue.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "getConsoleIdentifier.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
#include "scilabmode.h"
}

using namespace org_scilab_modules_gui_bridge;

int setMenuParent(char *pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    char *pstCurrentFigure = NULL;
    char *parentType = NULL;
    char *pParentUID = NULL;

    double *value = NULL;

    /* Special case to set current figure for parent */
    if (stackPointer == -1)
    {
        // Set the parent property
        pstCurrentFigure = getCurrentFigure();
        if (pstCurrentFigure == NULL)
        {
            pstCurrentFigure = createNewFigureWithAxes();
        }
        setGraphicObjectRelationship(pstCurrentFigure, pobjUID);
        return SET_PROPERTY_SUCCEED;
    }

    if (nbRow * nbCol != 1)
    {
        // Parent must be a single value
        return SET_PROPERTY_ERROR;
    }

    /* Check parent type */
    if (getScilabMode() == SCILAB_STD)
    {
        /* Figure, uimenu or Console can be the parent */
        if ((valueType == sci_handles) && (valueType == sci_matrix))
        {
            Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: A handle or 0 expected.\n")), "SetMenuParent");

            return 0;
        }
    }
    else
    {
        /* Figure, uimenu can be the parent */
        if (valueType == sci_matrix)
        {
            Scierror(999, const_cast < char *>(_("%s: can not add a menu into the console in this mode.\n")), "SetMenuParent");
            return 0;
        }
        else
        {
            Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: A handle expected.\n")), "SetMenuParent");
            return 0;
        }
    }

    if (valueType == sci_handles)
    {
        pParentUID = getObjectFromHandle(getHandleFromStack(stackPointer));
        if (pParentUID != NULL)
        {
            getGraphicObjectProperty(pParentUID, __GO_TYPE__, jni_string, (void **)&parentType);
            if ((strcmp(parentType, __GO_FIGURE__) == 0) || (strcmp(parentType, __GO_UIMENU__) == 0)
                    || (strcmp(parentType, __GO_UICONTEXTMENU__) == 0))
            {
                setGraphicObjectRelationship(pParentUID, pobjUID);
                free(parentType);
            }
            else
            {
                Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: Figure or uimenu handle expected.\n")), "SetMenuParent");

                free(parentType);
                return SET_PROPERTY_ERROR;
            }
        }
        else
        {
            Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: Figure or uimenu handle expected.\n")), "SetMenuParent");

            return SET_PROPERTY_ERROR;
        }
    }

    if (valueType == sci_matrix)
    {
        // The parent is Scilab Main window (Console Tab)
        value = getDoubleMatrixFromStack(stackPointer);
        if (value[0] != 0)
        {
            Scierror(999, const_cast < char *>(_("%s: Wrong value for parent: 0 expected.\n")), "SetMenuParent");

            return SET_PROPERTY_ERROR;
        }
        setGraphicObjectRelationship(getConsoleIdentifier(), pobjUID);
    }

    return SET_PROPERTY_SUCCEED;
}

void EnableMenu(char *pParentId, char *name, BOOL status)
{
    CallScilabBridge::setMenuEnabled(getScilabJavaVM(), pParentId, name, BOOLtobool(status));
}

void EnableSubMenu(char *pParentId, char *name, int position, BOOL status)
{
    CallScilabBridge::setSubMenuEnabled(getScilabJavaVM(), pParentId, name, position, BOOLtobool(status));
}

void DeleteMenuWithName(char *pParentId, char *name)
{
    CallScilabBridge::removeMenu(getScilabJavaVM(), pParentId, name);
}
