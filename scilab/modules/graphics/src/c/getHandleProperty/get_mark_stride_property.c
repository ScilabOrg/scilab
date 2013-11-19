/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_mark_stride_property.c                                       */
/* desc : function to retrieve in Scilab the mark_stride field of a       */
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
int get_mark_stride_property(void* _pvCtx, int iObjUID)
{
    int iMarkStride = 0;
    int* piMarkStride = &iMarkStride;

    getGraphicObjectProperty(iObjUID, __GO_MARK_STRIDE__, jni_int, (void**)&piMarkStride);

    if (piMarkStride == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "mark_stride");
        return -1;
    }

    return sciReturnDouble(_pvCtx, iMarkStride);
}
/*------------------------------------------------------------------------*/
