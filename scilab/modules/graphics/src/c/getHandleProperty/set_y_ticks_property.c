/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_y_ticks_property.c                                           */
/* desc : function to modify in Scilab the y_ticks field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "CheckTicksProperty.h"
#include "MALLOC.h"
#include "BasicAlgos.h"
#include "DrawObjects.h"
#include "freeArrayOfString.h"
#include "loadTextRenderingAPI.h"
#include "sci_types.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
/* @TODO: remove stackPointer, nbRow, nbCol which are used */
int set_y_ticks_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL autoTicks = FALSE;
    BOOL status = FALSE;
    AssignedList * tlist = NULL;
    int nbTicsRow = 0;
    int nbTicsCol = 0;

    double* userGrads = NULL;
    char** userLabels = NULL;

    if (!(valueType  == sci_tlist))
    {
        Scierror(999, _("Wrong type for '%s' property: Typed list expected.\n"), "y_ticks");
        return SET_PROPERTY_ERROR;
    }

    tlist = createTlistForTicks(_pvCtx);

    if (tlist == NULL)
    {
        return SET_PROPERTY_ERROR;
    }

    /* locations */
    userGrads = createCopyDoubleMatrixFromList(_pvCtx, tlist, &nbTicsRow, &nbTicsCol);

    if (userGrads == NULL && nbTicsRow == -1)
    {
        Scierror(999, _("%s: No more memory.\n"), "set_y_ticks_property");
        return SET_PROPERTY_ERROR;
    }

    /* Automatic ticks must be first deactivated in order to set user ticks */
    autoTicks = FALSE;

    setGraphicObjectProperty(iObjUID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);

    status = setGraphicObjectProperty(iObjUID, __GO_Y_AXIS_TICKS_LOCATIONS__, userGrads, jni_double_vector, nbTicsRow * nbTicsCol);

    if (status == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "y_ticks");
        FREE(userGrads);
        return SET_PROPERTY_ERROR;
    }

    /*  labels */
    // Here we check the size of "locations" instead of "labels", but they have the same size.
    // We need to check the size to not be 0 because an empty matrix is a matrix of double
    // and 'getCurrentStringMatrixFromList' expect a matrix of string (see bug 5148).
    // P.Lando
    if (nbTicsCol * nbTicsRow)
    {
        userLabels = getCurrentStringMatrixFromList(_pvCtx, tlist, &nbTicsRow, &nbTicsCol);
        /* Check if we should load LaTex / MathML Java libraries */
        loadTextRenderingAPI(userLabels, nbTicsCol, nbTicsRow);

        setGraphicObjectProperty(iObjUID, __GO_Y_AXIS_TICKS_LABELS__, userLabels, jni_string_vector, nbTicsRow * nbTicsCol);
    }
    else
    {
        /* To be implemented */
#if 0
        ppSubWin->axes.u_ylabels = NULL;
#endif
    }

    if (userGrads != NULL)
    {
        FREE(userGrads);
    }

    destroyAssignedList(tlist);

    return SET_PROPERTY_SUCCEED;

}
/*------------------------------------------------------------------------*/
