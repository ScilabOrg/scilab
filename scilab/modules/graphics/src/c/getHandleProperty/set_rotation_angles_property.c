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
/* file: set_rotation_angles_property.c                                   */
/* desc : function to modify in Scilab the rotation_angles field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_rotation_angles_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  BOOL status;
  double * values = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "rotation_angles");
    return SET_PROPERTY_ERROR ;
  }

#if 0
  /* DJ.A 2003 */
  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"rotation_angles") ;
    return SET_PROPERTY_ERROR ;
  }
#endif

  status = setGraphicObjectProperty(pobj->UID, __GO_ROTATION_ANGLES__, values, jni_double_vector, 2);

  if (status == TRUE)
  {
    return SET_PROPERTY_SUCCEED;
  }
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"rotation_angles") ;
    return SET_PROPERTY_ERROR;
  }

  /* deactivated for now since it involves re-drawing operations, to be implemented */
  #if 0
  Obj_RedrawNewAngle( pobj, values[0], values[1] ) ;
  #endif

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
