/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "MALLOC.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include "CallDatatipCreateField.h"

int set_datatips_property(void* _pvCtx, char* pobj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    double* currentDatatipsCoords;
    double* newDatatipsCoords;

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "datatips");
        return SET_PROPERTY_ERROR ;
    }

    if ( !( nbRow*nbCol % 3 == 0 ) )
    {
        Scierror(999, _("Wrong number of input arguments for '%s' property: (x, y, z) coordinates expected.\n"), "datatips");
        return SET_PROPERTY_ERROR ;
    }
    
    if ( nbCol != 3 )
    {
        Scierror(999, _("Wrong number of input arguments for '%s' property: (x, y, z) coordinates expected.\n"), "datatips");
	return SET_PROPERTY_ERROR ;
    }

    currentDatatipsCoords = (double*)_pvData;

    newDatatipsCoords = CallDatatipCreateField((char*)pobj, currentDatatipsCoords, nbRow * nbCol);

    status = setGraphicObjectProperty(pobj, __GO_DATATIPS__, newDatatipsCoords, jni_double_vector, nbRow * nbCol);

    FREE(newDatatipsCoords);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' does not exist for this handle.\n"), "datatips");
        return SET_PROPERTY_ERROR;
    }
}
