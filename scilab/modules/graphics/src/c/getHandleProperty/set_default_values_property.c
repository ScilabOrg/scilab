/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_default_values_property.c                                    */
/* desc : function to modify in Scilab the default_values field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "InitObjects.h"
#include "SetPropertyStatus.h"

#include "FigureModel.h"
#include "AxesModel.h"

/*------------------------------------------------------------------------*/
int set_default_values_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real expected.\n"), "default_values");
        return SET_PROPERTY_ERROR;
    }

    if (((double*)_pvData)[0] != 1)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be '%s'.\n"), "default_values", "1");
        return SET_PROPERTY_ERROR;
    }

    if ((iObjUID != 0) && isFigureModel(iObjUID))
    {
        InitFigureModel(iObjUID);
        return SET_PROPERTY_SUCCEED;
    }
    else if ((iObjUID != 0) && isAxesModel(iObjUID))
    {
        return InitAxesModel();
    }
    else if (iObjUID == 0)
    {
        /* set default values for current figure */
        return sciSetDefaultValues();
    }

    Scierror(999, _("'%s' property does not exist for this handle.\n"), "default_values");
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/
