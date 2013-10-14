/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_data_bounds_property.c                                       */
/* desc : function to retrieve in Scilab the data_bounds field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_data_bounds_property(void* _pvCtx, int iObjUID)
{
    double* dataBounds = NULL;
    int iView = 0;
    int* piView = &iView;

    getGraphicObjectProperty(iObjUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

    if (dataBounds == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "data_bounds");
        return -1;
    }

    getGraphicObjectProperty(iObjUID, __GO_VIEW__, jni_int, (void **)&piView);
    if (piView == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "view");
        return -1;
    }

    /**DJ.Abdemouche 2003**/
    if (iView == 1)
    {
        return sciReturnMatrix(_pvCtx, dataBounds, 2, 3);
    }
    else
    {
        return sciReturnMatrix(_pvCtx, dataBounds, 2, 2);
    }
}
/*------------------------------------------------------------------------*/
