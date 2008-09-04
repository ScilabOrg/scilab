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
/* file: set_old_style_property.c                                         */
/* desc : function to modify in Scilab the old_style field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_old_style_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  getStringFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"old_style") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return SET_PROPERTY_UNCHANGED ;
  }
  else if ( isStringParamEqual( stackPointer, "on" ) )
  {
    sciprint("Old graphic mode is no longer available. Please refer to the set help page.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  else
  {
    sciprint("old_style must be 'on' or 'off'.\n");
    return SET_PROPERTY_ERROR ;
  }
}
/*------------------------------------------------------------------------*/
