/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_line_width_property.c                                        */
/* desc : function to retrieve in Scilab the line_width field of a        */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_line_width_property(void* _pvCtx, int iObjUID)
{
    double dblLineWidth = 0.0;
    double *pdblLineWidth = &dblLineWidth;

    getGraphicObjectProperty(iObjUID, __GO_LINE_WIDTH__, jni_double, (void **)&pdblLineWidth);

    if (pdblLineWidth == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "line_width");
        return -1;
    }

    return sciReturnDouble(_pvCtx, dblLineWidth);
}
/*------------------------------------------------------------------------*/
