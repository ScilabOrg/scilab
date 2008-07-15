/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_viewport_property.c                                          */
/* desc : function to modify in Scilab the auto_rotation field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "GraphicSynchronizerInterface.h"

/*------------------------------------------------------------------------*/
int set_viewport_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int values[4];
  int status;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"viewport") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint(_("%s does not exist for this handle.\n"), "viewport") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 2 )
  {
    sciprint("Argument must be a vector of size 4.\n");
    return SET_PROPERTY_ERROR ;
  }

  /* For now we just use viewport positions */
  copyDoubleVectorToIntFromStack(stackPointer, values, 2);

  /* dummy values */
  values[2] = 0;
  values[3] = 0;

  /* force auto_resize. With auto_resize disable, resize does not work */

  /* disable protection since this function will call Java */
  endFigureDataWriting(pobj);
  status = sciSetViewport(pobj, values);
  startFigureDataWriting(pobj);

  return status;
}
/*------------------------------------------------------------------------*/
