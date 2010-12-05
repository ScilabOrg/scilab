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
/* file: set_text_renderer_property.c                                     */
/* desc : function to retrieve in Scilab the renderer field of a handle   */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_text_renderer_property(sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  if (sciGetEntityType(pobj) != SCI_TEXT && sciGetEntityType(pobj) != SCI_LEGEND && sciGetEntityType(pobj) != SCI_LABEL && sciGetEntityType(pobj) != SCI_SUBWIN)
  {
      Scierror(999, _("'%s' property does not exist for this handle.\n"), "renderer");
      return SET_PROPERTY_ERROR ;
  }

  if (!isParameterStringMatrix(valueType))
  {
      Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "renderer");
      return SET_PROPERTY_ERROR ;
  }

  if (nbCol != 1)
  {
      Scierror(999, _("Wrong size for '%s' property: %d string expected.\n"), "renderer", 1);
      return SET_PROPERTY_ERROR ;
  }

  return sciSetTextRenderer(pobj, getStringFromStack(stackPointer));
}
/*------------------------------------------------------------------------*/
