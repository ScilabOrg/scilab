/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * Sets the background of an uicontrol object
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolBackgroundColor.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolBackgroundColor(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Color can be [R, G, B] or "R|G|B" */

  int redInt = 0, greenInt = 0, blueInt = 0, nbvalues = 0;

  double * allcolors = NULL;

  double redDouble = 0.0, greenDouble = 0.0, blueDouble = 0.0;

  /*
  **  No backgroundcolor property except on UICONTROL
  */
  if(sciGetEntityType(sciObj) != SCI_UICONTROL)
    {
      sciprint(_("No '%s' property for this object.\n"), "BackgroundColor");
      return SET_PROPERTY_ERROR;
    }

  if (valueType == sci_strings)
    {
      if(nbCol != 1)
        {
          /* Wrong string size */
          sciprint(_("Wrong size for '%s' property: 1 x 3 real vector or a string expected.\n"), "BackgroundColor");
          return SET_PROPERTY_ERROR;
        }

      nbvalues = sscanf(getStringFromStack(stackPointer), "%lf|%lf|%lf", &redDouble, &greenDouble, &blueDouble);

      if (nbvalues != 3)
        {
          /* Wrong string format */
          sciprint(_("Wrong value for '%s' property: 1 x 3 real vector or a string expected.\n"), "BackgroundColor");
          return SET_PROPERTY_ERROR;
        }

      redInt = (int) (redDouble * 255);
      greenInt = (int) (greenDouble * 255);
      blueInt = (int) (blueDouble * 255);

      /* Store the values in Scilab */
      if (pUICONTROL_FEATURE(sciObj)->backgroundcolor == NULL)
        {
          pUICONTROL_FEATURE(sciObj)->backgroundcolor = new double[3];
        }
      pUICONTROL_FEATURE(sciObj)->backgroundcolor[0] = redDouble;
      pUICONTROL_FEATURE(sciObj)->backgroundcolor[1] = greenDouble;
      pUICONTROL_FEATURE(sciObj)->backgroundcolor[2] = blueDouble;
    }
  else if (valueType == sci_matrix)
    {
       if(nbCol != 3 || nbRow != 1)
        {
          /* Wrong matrix size */
          sciprint(_("Wrong size for '%s' property: 1 x 3 real vector or a string expected.\n"), "BackgroundColor");
          return SET_PROPERTY_ERROR;
        }

       allcolors = getDoubleMatrixFromStack(stackPointer);
       redInt = (int) (allcolors[0] * 255);
       greenInt = (int) (allcolors[1] * 255);
       blueInt = (int) (allcolors[2] * 255);

       /* Store the values in Scilab */
       if (pUICONTROL_FEATURE(sciObj)->backgroundcolor == NULL)
         {
           pUICONTROL_FEATURE(sciObj)->backgroundcolor = new double[3];
         }
       pUICONTROL_FEATURE(sciObj)->backgroundcolor[0] = allcolors[0];
       pUICONTROL_FEATURE(sciObj)->backgroundcolor[1] = allcolors[1];
       pUICONTROL_FEATURE(sciObj)->backgroundcolor[2] = allcolors[2];
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("Wrong type for '%s' property: 1 x 3 real vector or a string expected.\n"), "BackgroundColor");
      return SET_PROPERTY_ERROR;
    }

  if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
    {
      CallScilabBridge::setFrameBackgroundColor(getScilabJavaVM(),
                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                 redInt, greenInt, blueInt);
    }
  else /* All other uicontrol styles */
    {
      CallScilabBridge::setWidgetBackgroundColor(getScilabJavaVM(),
                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                 redInt, greenInt, blueInt);
    }
  return SET_PROPERTY_SUCCEED;
}

