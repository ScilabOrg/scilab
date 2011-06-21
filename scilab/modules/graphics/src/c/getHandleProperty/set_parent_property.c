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
/* file: set_parent_property.c                                            */
/* desc : function to modify in Scilab the parent field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include "sci_types.h"
#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "InitUIMenu.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_parent_property(char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    char* figureUID = NULL;
    char* type = NULL;

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_string, &type);

    if (strcmp(type, __GO_UICONTROL__) == 0)
    {
        if (valueType == sci_handles)
        {
            figureUID = getObjectFromHandle(getHandleFromStack(stackPointer));
        }
        else if (valueType == sci_matrix)
        {
            figureUID = getFigureFromIndex((int)getDoubleMatrixFromStack(stackPointer)[0]);
        }
        else
        {
          Scierror(999,_("Wrong type for '%s' property: '%s' handle or '%s' handle expected.\n"),"parent","Figure", "Frame uicontrol");
          return SET_PROPERTY_ERROR;
        }

        if (figureUID == NULL)
        {
            // Can not set the parent
            Scierror(999, _("Wrong value for '%s' property: A '%s' or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
            return SET_PROPERTY_ERROR;
        }

        setGraphicObjectRelationship(figureUID, pobjUID);
    }

#if 0
  if(sciGetEntityType( pobj ) == SCI_UIMENU)
    {
      if ((pobj == NULL) || (valueType!=sci_handles && valueType!=sci_matrix)) /* sci_matrix used for adding menus in console menu */
        {
          Scierror(999,_("Wrong type for '%s' property: '%s' handle or '%s' handle expected.\n"),"parent","Figure", "Uimenu");
          return SET_PROPERTY_ERROR ;
        }
      else
        {
          return setMenuParent(pobj, stackPointer, valueType, nbRow, nbCol);
        }
    }
  else if(sciGetEntityType(pobj) == SCI_UICONTROL)
    {
      if ((pobj == NULL) || (valueType!=sci_handles && valueType!=sci_matrix))
        {
          Scierror(999,_("Wrong type for '%s' property: '%s' handle or '%s' handle expected.\n"),"parent","Figure", "Uimenu");
          return SET_PROPERTY_ERROR ;
        }
      else
        {
          return SetUicontrolParent(pobj, stackPointer, valueType, nbRow, nbCol);
        }
    }
  else
    {
      Scierror(999, _("Parent property can not be modified directly.\n"));
      return SET_PROPERTY_ERROR ;
    }
#endif
}
/*------------------------------------------------------------------------*/
