/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_triangles_property.c                                         */
/* desc : function to modify in Scilab the surface_color field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_triangles_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int iType = -1;
    int *piType = &iType;
    BOOL result = FALSE;
    double* pnoeud = NULL;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "triangles");
        return SET_PROPERTY_ERROR;
    }

    /*
     * Discriminating between a failed allocation and the non-existing property case
     * is not done for now.
     * To be implemented/corrected.
     */
    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType == __GO_FEC__)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "triangles");
        return SET_PROPERTY_ERROR;
    }

    if (nbCol < 5)
    {
        Scierror(999, _("Wrong size for '%s' property: Must have at least %d columns.\n"), "triangles", 5);
        return SET_PROPERTY_ERROR;
    }

    result = setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, &nbCol, jni_int, 1);
    if (result == FALSE)
    {
        Scierror(999, _("%s: No more memory.\n"), "set_triangles_property");
        return 0;
    }

    /* Resizes the triangle array if required */
    result  = setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_NUM_INDICES__, &nbRow, jni_int, 1);

    if (result == FALSE)
    {
        Scierror(999, _("%s: No more memory.\n"), "set_triangles_property");
        return SET_PROPERTY_ERROR;
    }

    pnoeud = (double*)_pvData;

    setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_FEC_ELEMENTS__, pnoeud, jni_double_vector, nbRow);

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
