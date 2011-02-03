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
/* file: get_auto_clear_property.c                                        */
/* desc : function to retrieve in Scilab the auto_clear field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_auto_clear_property( sciPointObj * pobj )
{
    int iAutoClear = 0;
    int* piAutoClear = &iAutoClear;

#if 0
    if (   sciGetEntityType(pobj) != SCI_SUBWIN
           && sciGetEntityType(pobj) != SCI_FIGURE )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_clear property") ;
        return -1 ;
    }
#endif

    getGraphicObjectProperty(pobj->UID, __GO_AUTO_CLEAR__, jni_bool, &piAutoClear);

    if (piAutoClear == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_clear property");
        return -1;
    }

    if (iAutoClear)
    {
        return sciReturnString( "on" );
    }
    else
    {
        return sciReturnString( "off" );
    }
}
/*------------------------------------------------------------------------*/
