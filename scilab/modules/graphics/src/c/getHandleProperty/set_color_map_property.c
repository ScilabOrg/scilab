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
/* file: set_color_map_property.c                                         */
/* desc : function to modify in Scilab the color_map field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_color_map_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Incompatible type for property %s.\n"),"color_map") ;
    return SET_PROPERTY_ERROR ;
  }

  return sciSetColormap( pobj, getDoubleMatrixFromStack( stackPointer), nbRow, nbCol ) ;
}
/*------------------------------------------------------------------------*/
