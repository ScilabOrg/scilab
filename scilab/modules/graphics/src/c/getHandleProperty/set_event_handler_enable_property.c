/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
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
/* file: set_event_handler_enable_property.c                              */
/* desc : function to modify in Scilab the event_handler_enable field of  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_event_handler_enable_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
	int b =  (int)FALSE;
	BOOL status;

#if 0
	if ( sciGetEntityType( pobj ) != SCI_FIGURE )
	{
		Scierror(999, _("'%s' property does not exist for this handle.\n"),"event_handler_enable");
		return SET_PROPERTY_ERROR ;
	}
#endif

	b =  tryGetBooleanValueFromStack(stackPointer, valueType, nbRow, nbCol, "event_handler_enable");
	if (b == NOT_A_BOOLEAN_VALUE) {
		return SET_PROPERTY_ERROR;
	}

	status = setGraphicObjectProperty(pobj->UID, __GO_EVENTHANDLER_ENABLE__, &b, jni_bool, 1);

	if (status == TRUE)
	{
		return SET_PROPERTY_SUCCEED;
	}
	else
	{
		Scierror(999, _("'%s' property does not exist for this handle.\n"),"event_handler_enable");
		return SET_PROPERTY_ERROR;
	}

/* deactivated for now sinces it involves drawing operations, to be implemented */
#if 0
	return (int)sciSetNoRedrawStatus((SetPropertyStatus)sciSetIsEventHandlerEnable(pobj, b));
#endif
}
/*------------------------------------------------------------------------*/
