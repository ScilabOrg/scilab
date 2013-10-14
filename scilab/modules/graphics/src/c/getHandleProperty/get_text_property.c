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
/* file: get_text_property.c                                              */
/* desc : function to retrieve in Scilab the text field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "StringMatrix.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------*/
int get_text_property(void* _pvCtx, int iObjUID)
{
    int* dimensions;
    char** textMatrix;

    getGraphicObjectProperty(iObjUID, __GO_TEXT_ARRAY_DIMENSIONS__, jni_int_vector, (void **) &dimensions);

    if (dimensions == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "text");
        return -1;
    }

    getGraphicObjectProperty(iObjUID, __GO_TEXT_STRINGS__, jni_string_vector, (void **) &textMatrix);

    if (textMatrix == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "text");
        return -1;
    }

    return sciReturnStringMatrix(_pvCtx, textMatrix, dimensions[0], dimensions[1]);
}
/*------------------------------------------------------------------------*/
