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
/* file: set_figure_style_property.c                                      */
/* desc : function to modify in Scilab the figure_style field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "stricmp.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
int set_figure_style_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "figure_style");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "old") == 0)
    {
        Scierror(999, _("Old graphic mode is no longer available. Please refer to the set help page.\n"));
        return SET_PROPERTY_ERROR;
    }
    else if (stricmp((char*)_pvData, "new") == 0)
    {
        /* graphic mode must be new */
        /* nothing to do */
        return SET_PROPERTY_UNCHANGED;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "figure_style", "'old'", "'new'");

    }

    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/
