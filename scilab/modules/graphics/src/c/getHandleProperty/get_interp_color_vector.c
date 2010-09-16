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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_interp_color_vector_property.c                               */
/* desc : function to retrieve in Scilab the interp_color_vector field of */
/*        a handle.                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
int get_interp_color_vector_property( sciPointObj * pobj )
{
    int* interpVector = NULL;
    int* interpVectorSet; 
    int* numElements;

#if 0
    if( sciGetEntityType(pobj) != SCI_POLYLINE )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"interp_color_vector");
        return -1;
    }
#endif

    interpVectorSet = (int*) getGraphicObjectProperty(pobj->UID, __GO_INTERP_COLOR_VECTOR_SET__, jni_bool);

    if (interpVectorSet == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"interp_color_vector");
        return -1;
    }

    if (*interpVectorSet == FALSE)
    {
        return sciReturnEmptyMatrix();
    }
    else
    {
        interpVector = (int*) getGraphicObjectProperty(pobj->UID, __GO_INTERP_COLOR_VECTOR__, jni_int_vector);
        numElements = (int*) getGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int);

        return sciReturnRowVectorFromInt(interpVector, *numElements);
    }
}
/*--------------------------------------------------------------------------*/
