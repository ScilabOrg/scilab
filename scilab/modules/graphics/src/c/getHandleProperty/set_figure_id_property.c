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
/* file: set_figure_id_property.c                                         */
/* desc : function to modify in Scilab the figure_id field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "InitObjects.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_figure_id_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int id = (int) getDoubleFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"figure_id") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint(_("%s property undefined for this object.\n"), "figure_id") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( pobj != getFigureModel() )
  {
    return sciInitUsedWindow( id ) ;
  }
  else
  {
    return sciSetNum( getFigureModel(), id ) ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
