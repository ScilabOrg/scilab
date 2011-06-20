/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "Slider.hxx"

using namespace org_scilab_modules_gui_bridge;

void createSlider(sciPointObj* sciObj)
{
  pUICONTROL_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newSlider(getScilabJavaVM());
}

int setCurentFigureAsSliderParent(sciPointObj* sciObj)
{
// ???
#if  0
  int parentFigureIndex = 0;

  // Scilab list of parent
  sciAddThisToItsParent(sciObj, sciGetCurrentFigure());

  // Java objects
  parentFigureIndex = sciGetNum(sciGetCurrentFigure());
  CallScilabBridge::setSliderParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
#endif
  return SET_PROPERTY_SUCCEED;
}
