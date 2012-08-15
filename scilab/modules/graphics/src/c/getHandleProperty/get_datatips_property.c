/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_datatips_property.c                                          */
/* desc: function to retrieve in Scilab the datatips field of a handle    */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_datatips_property(void* _pvCtx, char* pobjUID)
{
    int iDatatips = 0;
    int *piDatatips = &iDatatips;

    getGraphicObjectProperty(pobjUID, __GO_DATATIPS__, jni_int, (void**)&piDatatips);

    if (piDatatips == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "datatips");
        return -1;
    }

    return sciReturnDouble(_pvCtx, iDatatips);
}
/*------------------------------------------------------------------------*/
