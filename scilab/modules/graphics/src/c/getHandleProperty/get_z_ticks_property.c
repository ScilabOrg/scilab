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
/* file: get_z_ticks_property.c                                           */
/* desc : function to retrieve in Scilab the z_ticks field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "get_ticks_utils.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_z_ticks_property( sciPointObj * pobj )
{
  int* tmp;
  int nbTicks;

#if 0
  if ( sciGetEntityType( pobj ) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"z_ticks");
    return -1;
  }
#endif

  /* retrieve number of ticks */
  tmp = (int*) getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_NUMBER_TICKS__, jni_int);

  if (tmp == NULL)
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"z_ticks");
    return -1;
  }

  nbTicks = *tmp;

  if (nbTicks == 0)
  {
    /* return empty matrices */
    buildTListForTicks( NULL, NULL, 0);
  }
  else
  {
    char ** labels;
    double * positions;

    positions = (double*)getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_TICKS_LOCATIONS__, jni_double_vector);

    labels = (char**)getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_TICKS_LABELS__, jni_string_vector);

    if (positions == NULL || labels == NULL)
    {
      Scierror(999, _("'%s' property does not exist for this handle.\n"),"z_ticks");
      return -1;
    }

    buildTListForTicks( positions, labels, nbTicks );

    /* free arrays */
#if 0
    destroyStringArray(labels, nbTicks);
    FREE(positions);
#endif
  }

  return 0;

}
/*------------------------------------------------------------------------*/
