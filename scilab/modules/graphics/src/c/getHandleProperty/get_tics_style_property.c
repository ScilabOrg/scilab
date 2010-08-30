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
/* file: get_tics_style_property.c                                        */
/* desc : function to retrieve in Scilab the tics_style field of          */
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
int get_tics_style_property( sciPointObj * pobj )
{
    int* tmpTicksStyle;
    char ticksStyle;

#if 0
    if ( sciGetEntityType( pobj ) != SCI_AXES )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tics_style");
        return -1;
    }
#endif

    tmpTicksStyle = (int*) getGraphicObjectProperty(pobj->UID, __GO_TICKS_STYLE__, jni_int);

    if (tmpTicksStyle == NULL)
    {
        Scierror(999, _("Wrong value for '%s' property.\n"),"tics_direction");
        return -1;
    }

    if (*tmpTicksStyle == 0)
    {
        ticksStyle = 'v';
    }
    else if (*tmpTicksStyle == 1)
    {
        ticksStyle = 'r';
    }
    else if (*tmpTicksStyle == 2)
    {
        ticksStyle = 'i';
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"),"tics_style");
        return -1;
    }

    return sciReturnChar(ticksStyle);

}
/*------------------------------------------------------------------------*/
