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
/* file: set_interp_color_mode_property.c                                 */
/* desc : function to modify in Scilab the interp_color_mode field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_interp_color_mode_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"interp_color_mode") ;
    return SET_PROPERTY_ERROR ;
  }

  if( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint("interp_color_mode can only be set on Polyline objects.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if( isStringParamEqual( stackPointer, "on" ) )
  {
    if ( sciGetInterpVector(pobj) == NULL )
    {
      sciprint("You must first specify an interp_color_vector for this object.\n") ;
      return SET_PROPERTY_ERROR ;
    }
    else
    {
      pPOLYLINE_FEATURE (pobj)->isinterpshaded = TRUE ;
    }
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    pPOLYLINE_FEATURE (pobj)->isinterpshaded = FALSE ;
  }
  else
  {
    sciprint("Property must be set to 'on' or 'off'.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
