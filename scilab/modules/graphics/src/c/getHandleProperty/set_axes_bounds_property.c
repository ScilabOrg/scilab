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
/* file: set_axes_bounds_property.c                                       */
/* desc : function to modify in Scilab the axes_bounds field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"
//#include "PloEch.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_axes_bounds_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  BOOL status = FALSE;
  double axesBounds[4];

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "axes_bounds");
    return SET_PROPERTY_ERROR ;
  }

#if 0
  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"axes_bounds") ;
    return SET_PROPERTY_ERROR ;
  }
#endif

  if ( nbRow * nbCol != 4 )
  {
    Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "axes_bounds", 4);
    return SET_PROPERTY_ERROR ;
  }

  copyDoubleVectorFromStack( stackPointer, axesBounds, 4 );

  status = setGraphicObjectProperty(pobjUID, __GO_AXES_BOUNDS__, axesBounds, jni_double_vector, 4);

  if (status == TRUE)
  {
    return SET_PROPERTY_SUCCEED;
  }
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"axes_bounds");
    return SET_PROPERTY_ERROR;
  }
}
/*------------------------------------------------------------------------*/
