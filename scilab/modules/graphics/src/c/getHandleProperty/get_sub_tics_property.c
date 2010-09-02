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
/* file: get_sub_tics_property.c                                          */
/* desc : function to retrieve in Scilab the sub_tics or sub_ticks field  */
/*        of a handle                                                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_sub_tics_property( sciPointObj * pobj )
{
  char* type;

  /*Dj.A 17/12/2003*/
  /* modified jb Silvy 01/2006 */

  type = (char*) getGraphicObjectProperty(pobj->UID, __GO_TYPE__, jni_string);

  /*
   * Type test required as the Axis object stores subticks as a single int
   * whereas Axes maintain a 3-element int vector.
   */
  if (strcmp(type, __GO_AXIS__) == 0)
  {
    int* subTicks;

    subTicks = (int*) getGraphicObjectProperty(pobj->UID, __GO_SUBTICKS__, jni_int);

    if (subTicks == NULL)
    {
      Scierror(999, _("'%s' property does not exist for this handle.\n"),"sub_ticks");
      return -1;
    }

    return sciReturnDouble(*subTicks);
  }
  else if (strcmp(type, __GO_AXES__) == 0)
  {
    double sub_ticks[3];
    int* subTicks;
    int* view;

    subTicks = (int*) getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, jni_int);

    if (subTicks == NULL)
    {
      Scierror(999, _("'%s' property does not exist for this handle.\n"),"sub_ticks");
      return -1;
    }

    sub_ticks[0] = *subTicks;

    subTicks = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int);
    sub_ticks[1] = *subTicks;

    subTicks = (int*) getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_SUBTICKS__, jni_int);
    sub_ticks[2] = *subTicks;

    view = (int*) getGraphicObjectProperty(pobj->UID, __GO_VIEW__, jni_int);

    if (*view == 1)
    {
      return sciReturnRowVector( sub_ticks, 3 );
    }
    else
    {
      return sciReturnRowVector( sub_ticks, 2 );
    }
  }
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"sub_ticks");
    return -1 ;
  }
}
/*------------------------------------------------------------------------*/
