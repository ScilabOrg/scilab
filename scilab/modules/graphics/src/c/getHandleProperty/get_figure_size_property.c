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
/* file: get_figure_size_property.c                                       */
/* desc : function to retrieve in Scilab the figure_size field of a       */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_figure_size_property( sciPointObj * pobj )
{
  double figureSize[2] ;
  int* intSize;

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"figure_size");
    return -1;
  }

  intSize = (int*)getGraphicObjectProperty(pobj->UID, __GO_SIZE__, jni_int_vector);

  figureSize[0] = (double)intSize[0];
  figureSize[1] = (double)intSize[1];

  return sciReturnRowVector(figureSize, 2);
}
/*------------------------------------------------------------------------*/
