/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Pierre Lando <pierre.lando@scilab.org>
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_links_property.c                                             */
/* desc : function to modify in Scilab the related polyline to a legend.  */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_links_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status;
    char* type;
    char* parentAxes;
    char** links;
    int* tmp;
    int nblegends;
    int i;
    int iLinksCount = 0;
    int* piLinksCount = &iLinksCount;

    if ( !isParameterHandle( valueType ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Graphic handle array expected.\n"), "links");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(pobj->UID, __GO_LINKS_COUNT__, jni_int, &piLinksCount);

    if (piLinksCount == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"links");
        return SET_PROPERTY_ERROR;
    }

    nblegends = *tmp;

    if (nbRow*nbCol != nblegends)
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "links", nblegends);
        return SET_PROPERTY_ERROR;
    }

    links = (char**) MALLOC(nblegends*sizeof(char*));

    if (links == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"),"set_z_ticks_property");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(pobj->UID, __GO_PARENT_AXES__, jni_string, parentAxes);

    status = TRUE;

    for (i=0; i<nblegends; i++)
    {
        char* polylineParentAxes;
        sciPointObj* polylineObject = sciGetPointerFromHandle( getHandleFromStack( stackPointer+i ) );

        getGraphicObjectProperty(polylineObject->UID, __GO_TYPE__, jni_string, &type);

        if (strcmp(type, __GO_POLYLINE__) != 0)
        {
            Scierror(999, _("%s: Input argument #%d must be a '%s' handle.\n"), "links", i, "polyline");
            status = FALSE;
            break;
        }

        links[i] = polylineObject->UID;

        getGraphicObjectProperty(pobj->UID, __GO_PARENT_AXES__, jni_string, polylineParentAxes);

        if (strcmp(polylineParentAxes, parentAxes) != 0)
        {
            Scierror(999, _("%s: Input argument and the legend must have the same parent axes.\n"), "links");
            status = FALSE;
            break;
        }
    }

    if (status == FALSE)
    {
        FREE(links);
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(pobj->UID, __GO_LINKS__, links, jni_string_vector, nblegends);

    FREE(links);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"links");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
