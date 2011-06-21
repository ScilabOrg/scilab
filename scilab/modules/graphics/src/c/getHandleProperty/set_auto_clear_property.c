/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
/* file: set_auto_clear_property.c                                        */
/* desc : function to modify in Scilab the auto_clear field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "CurrentSubwin.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_auto_clear_property(char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
	BOOL status;
	int b =  (int)FALSE;
    char* objUID = NULL;

	if (pobjUID == NULL)
	{
		objUID = getCurrentSubWin();
	}
    else
    {
        objUID = pobjUID;
    }

	b = tryGetBooleanValueFromStack(stackPointer, valueType, nbRow, nbCol, "auto_clear");

	if(b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

	status = setGraphicObjectProperty(objUID, __GO_AUTO_CLEAR__, &b, jni_bool, 1);

	if (status == TRUE)
	{
		return SET_PROPERTY_SUCCEED;
	}
	else
	{
		Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_clear");
		return SET_PROPERTY_ERROR;
	}
}
/*------------------------------------------------------------------------*/

