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
/* file: set_font_color_property.c                                        */
/* desc : function to modify in Scilab the font_color field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_font_color_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  int value = (int) getDoubleFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"font_color") ;
    return SET_PROPERTY_ERROR ;
  }

  if (   sciGetEntityType(pobj) == SCI_SUBWIN
           || sciGetEntityType(pobj) == SCI_FIGURE
           || sciGetEntityType(pobj) == SCI_LEGEND
           || sciGetEntityType(pobj) == SCI_AXES)
  {
    return sciSetFontForeground( pobj,value ) ;
  } /* F.Leray 08.04.04 */
  else
  {
    sciprint(_("%s property does not exist for this handle.\n"),"font_color") ;
    return -1 ;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
