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
/* file: get_axes_visible_property.c                                      */
/* desc : function to retrieve in Scilab the axes_visible field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_axes_visible_property( sciPointObj * pobj )
{

  char * axes_visible[3]  = { NULL, NULL, NULL };
  char* axesVisiblePropertiesNames[3] = {__GO_X_AXIS_VISIBLE__, __GO_Y_AXIS_VISIBLE__, __GO_Z_AXIS_VISIBLE__};

  int i ;
  int status = -1;

#if 0
  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"axes_visible");
    return -1;
  }
#endif

  for ( i = 0 ; i < 3 ; i++ )
  {
    int* axesVisible;

    axes_visible[i] = MALLOC( 4 * sizeof(char) );
    if ( axes_visible[i] == NULL )
    {
      int j ;
      for ( j = 0 ; j < i ; j++ )
      {
        FREE( axes_visible[j] );
	Scierror(999, _("%s: No more memory.\n"),"get_axes_visible_property");
        return -1 ;
      }
    }

    axesVisible = (int*) getGraphicObjectProperty(pobj->UID, axesVisiblePropertiesNames[i], jni_bool);

    if (axesVisible == NULL) {
      int j;

      for ( j = 0 ; j < i ; j++ )
      {
        FREE(axes_visible[j]);
      }

      Scierror(999, _("'%s' property does not exist for this handle.\n"),"axes_visible");
      return -1;
    }

    if (*axesVisible)
    {
      strcpy( axes_visible[i], "on" );
    }
    else
    {
      strcpy( axes_visible[i], "off" );
    }
  }

  status = sciReturnRowStringVector( axes_visible, 3 );

  for ( i = 0 ; i < 3 ; i++ )
  {
    FREE( axes_visible[i] );
  }

  return status ;
}
/*------------------------------------------------------------------------*/
