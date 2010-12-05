/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_text_renderer_property.c                                     */
/* desc : function to retrieve in Scilab the renderer field of a handle   */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_text_renderer_property( sciPointObj * pobj )
{
  if (sciGetEntityType(pobj) != SCI_TEXT && sciGetEntityType(pobj) != SCI_LABEL && sciGetEntityType(pobj) != SCI_LEGEND && sciGetEntityType(pobj) != SCI_SUBWIN)
  {
      Scierror(999, _("'%s' property does not exist for this handle.\n"), "renderer") ;
      return -1 ;
  }

  return sciReturnString(sciGetTextRenderer(pobj));
}
/*------------------------------------------------------------------------*/
