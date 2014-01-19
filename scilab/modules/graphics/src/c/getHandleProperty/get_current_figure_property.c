/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
/* file: get_current_figure_property.c                                    */
/* desc : function to retrieve in Scilab the current_figure field of a    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include <stdio.h>

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

#include "createGraphicObject.h"
#include "BuildObjects.h"
#include "CurrentFigure.h"
#include "HandleManagement.h"

/*--------------------------------------------------------------------------*/
int get_current_figure_property(void* _pvCtx, int iObjUID)
{
    int iCurrentFigureId = 0;

    if (iObjUID != 0)
    {
        /* This property should not be called on an handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_figure");
        return -1;
    }

    iCurrentFigureId = getCurrentFigure();

    if (iCurrentFigureId == 0)
    {
        /* return handle on the current figure if none : create it */
        iCurrentFigureId = createNewFigureWithAxes();
        setCurrentFigure(iCurrentFigureId);
    }

    return sciReturnHandle(_pvCtx, getHandle(iCurrentFigureId));

}
