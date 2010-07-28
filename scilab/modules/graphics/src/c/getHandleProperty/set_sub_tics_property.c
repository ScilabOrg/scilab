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
/* file: set_sub_tics_property.c                                          */
/* desc : function to modify in Scilab the sub_tics field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_sub_tics_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  BOOL status;
  BOOL result;
  char* type;
  char* axisSubticksPropertiesNames[3] = {__GO_X_AXIS_SUBTICKS__, __GO_Y_AXIS_SUBTICKS__, __GO_Z_AXIS_SUBTICKS__};

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "sub_tics");
    return SET_PROPERTY_ERROR ;
  }

  type = (char*) getGraphicObjectProperty(pobj->UID, __GO_TYPE__, jni_string);

  if (strcmp(type, __GO_AXIS__) == 0)
  {
    int nbTicks = (int) getDoubleFromStack(stackPointer);

    status =  setGraphicObjectProperty(pobj->UID, __GO_SUBTICKS__, &nbTicks, jni_int, 1);

    if (status == TRUE)
    {
      return SET_PROPERTY_ERROR;
    }
    else
    {
      Scierror(999, _("'%s' property does not exist for this handle.\n"),"sub_tics");
      return SET_PROPERTY_SUCCEED;
    }
  }
  else if (strcmp(type, __GO_AXES__) == 0)
  {
    int i ;
    double * values = getDoubleMatrixFromStack( stackPointer );

    /* To be deleted, see the flagNax line below */
    sciSubWindow * ppSubWin = pSUBWIN_FEATURE (pobj);

    result = SET_PROPERTY_SUCCEED;

    if ( (nbCol != 3 ) && (nbCol != 2) )
    {
      Scierror(999, _("Wrong size for '%s' property: %d or %d elements expected.\n"), "sub_tics", 2, 3);
      return  SET_PROPERTY_ERROR ;
    }

    /* To be implemented / modified */
    ppSubWin->flagNax = TRUE;

    for ( i = 0; i < nbCol ; i++ )
    {
      int nbTicks;

      nbTicks = (int) values[i];

      if( nbTicks < 0 )
      {
        nbTicks = 0; 
      }

      status = setGraphicObjectProperty(pobj->UID, axisSubticksPropertiesNames[i], &nbTicks, jni_int, 1);

      if (status == FALSE)
      {
        result = SET_PROPERTY_ERROR;
      }
    }

    if (result == SET_PROPERTY_ERROR)
    {
      Scierror(999, _("'%s' property does not exist for this handle.\n"),"sub_ticks") ;
    }

    return result;
  }
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"sub_ticks") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
