/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
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
/* file: get_drawing_method_property.c                                    */
/* desc : function to retrieve in Scilab the drawing_method field of      */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_arc_drawing_method_property( sciPointObj * pobj )
{
    int iArcDrawingMethod = 0;
    int* piArcDrawingMethod = &iArcDrawingMethod;

#if 0
    if ( sciGetEntityType(pobj) != SCI_ARC && sciGetEntityType(pobj) != SCI_SUBWIN )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"arc_drawing_method");
        return -1;
    }
#endif

    getGraphicObjectProperty(pobj->UID, __GO_ARC_DRAWING_METHOD__, jni_int, &piArcDrawingMethod);

    if (piArcDrawingMethod == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"arc_drawing_method");
        return -1;
    }

    /* 0: nurbs, 1: lines */
    if (iArcDrawingMethod == 0)
    {
        return sciReturnString("nurbs");
    }
    else if (iArcDrawingMethod == 1)
    {
        return sciReturnString("lines");
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"), "arc_drawing_method");
        return -1;
    }

}
/*------------------------------------------------------------------------*/
