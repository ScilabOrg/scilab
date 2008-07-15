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
/* file: set_axes_size_property.c                                         */
/* desc : function to modify in Scilab the axes_size field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "InitObjects.h"
#include "SetPropertyStatus.h"
#include "GraphicSynchronizerInterface.h"

/*------------------------------------------------------------------------*/
int set_axes_size_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  double * newWindowSize = getDoubleMatrixFromStack( stackPointer ) ;
  int status;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"axes_size") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint(_("%s undefined for this object.\n"), "axes_size") ;
    return SET_PROPERTY_ERROR ;
  }

  /* disable protection since this function will call Java */
  endFigureDataWriting(pobj);
  status = sciSetDimension(pobj, (int) newWindowSize[0], (int) newWindowSize[1] ) ;
  startFigureDataWriting(pobj);

  if (status == SET_PROPERTY_ERROR)
  {
    sciprint(_("WARNING: '%s' property can not be modified if the %s is docked with other elements.\n"), "axes_size", "Figure") ;
    return SET_PROPERTY_ERROR ;
  }

  return status;
}
/*------------------------------------------------------------------------*/
