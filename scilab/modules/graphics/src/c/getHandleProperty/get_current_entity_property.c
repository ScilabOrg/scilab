/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_current_entity_property.c                                    */
/* desc : function to retrieve in Scilab the current_entity field of a    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "HandleManagement.h"
#include "returnProperty.h"
#include "CurrentObject.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
#include "BuildObjects.h"

/*--------------------------------------------------------------------------*/
void* get_current_entity_property(void* _pvCtx, int iObjUID)
{
    if (iObjUID != 0)
    {
        /* This property should not be called on an handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_entity");
        return NULL;
    }
    /* To be sure that there is at least one opened figure */
    getOrCreateDefaultSubwin();

    return sciReturnHandle(getHandle(getCurrentObject()));
}
/*--------------------------------------------------------------------------*/
