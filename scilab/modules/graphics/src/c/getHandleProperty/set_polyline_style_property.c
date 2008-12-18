/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_polyline_style_property.c                                    */
/* desc : function to modify in Scilab the polyline_style field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_polyline_style_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int value = 0 ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Incompatible type for property %s.\n"),"polyline_style") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) != SCI_POLYLINE)
  {
    Scierror(999, _("%s property does not exist for this handle.\n"),"polyline_style") ;
    return SET_PROPERTY_ERROR ;
  }
    
  value = (int) getDoubleFromStack( stackPointer ) ;
  if ( value < 1 || value > 7 )
  {
    Scierror(999, _("Wrong value for %s property: Must be between %d and %d.\n"),"Style",1,7);
    return SET_PROPERTY_ERROR ;
  }

  pPOLYLINE_FEATURE (pobj)->plot = value ;
  return SET_PROPERTY_SUCCEED ;
  
}
/*------------------------------------------------------------------------*/
