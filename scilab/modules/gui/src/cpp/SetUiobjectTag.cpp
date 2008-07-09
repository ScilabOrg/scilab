/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Set the tag of an uicontrol or uimenu
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUiobjectTag.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUiobjectTag(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Label must be only one character string
  if (valueType != sci_strings) {
    sciprint(_("Wrong value for '%s' property: A string expected.\n"), "Tag");
    return SET_PROPERTY_ERROR;
  }
  if (nbCol != 1) {
    sciprint(_("Wrong size for '%s' property: A string expected.\n"), "Tag");
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // No test needed about the style because not implemented in Java code

      // Free old tag is exists
      if(pUIMENU_FEATURE(sciObj)->tag != NULL)
        {
          delete (pUIMENU_FEATURE(sciObj)->tag);
        }
      // Set the new tag
      pUIMENU_FEATURE(sciObj)->tag = new char[strlen(getStringFromStack(stackPointer)) + 1];
      strcpy(pUIMENU_FEATURE(sciObj)->tag, getStringFromStack(stackPointer));

      return SET_PROPERTY_SUCCEED;
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // No test needed about the style because not implemented in Java code

      // Free old tag is exists
      if(pUICONTROL_FEATURE(sciObj)->tag != NULL)
        {
          delete (pUICONTROL_FEATURE(sciObj)->tag);
        }
      // Set the new tag
      pUICONTROL_FEATURE(sciObj)->tag = new char[strlen(getStringFromStack(stackPointer)) + 1];
      strcpy(pUICONTROL_FEATURE(sciObj)->tag, getStringFromStack(stackPointer));
      
      return SET_PROPERTY_SUCCEED;
    }
  else if (sciGetEntityType( sciObj ) == SCI_FIGURE)
    {
      // No test needed about the style because not implemented in Java code

      // Free old tag is exists
      if(pFIGURE_FEATURE(sciObj)->tag != NULL)
        {
          delete (pFIGURE_FEATURE(sciObj)->tag);
        }
      // Set the new tag
      pFIGURE_FEATURE(sciObj)->tag = new char[strlen(getStringFromStack(stackPointer)) + 1];
      strcpy(pFIGURE_FEATURE(sciObj)->tag, getStringFromStack(stackPointer));
      
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      sciprint(_("No '%s' property for this object.\n"), "Tag");
      return SET_PROPERTY_ERROR;
    }
}

