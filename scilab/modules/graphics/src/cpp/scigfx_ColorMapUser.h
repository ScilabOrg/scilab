/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
/* file: scigfx_ColorMapUser.h                                                */
/* desc : declare the class ColorMapUser                                      */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#ifndef _SCI_COLORMAPUSER_H_
#define _SCI_COLORMAPUSER_H_

  // Include nécessaire pour sciGraphics::DrawableObject
/*----------------------------------------------------------------------------*/
#include "DrawableObject.h"

  // Include nécessaire pour sciGetNumColors
/*----------------------------------------------------------------------------*/
#include "GetProperty.h"



class ColorMapUser:
  virtual public sciGraphics::DrawableObject
{
public:
  ColorMapUser(sciPointObj * pObj);

  int sci2cppColorIndex(int colorindex);

  int getColorMapSize();
};


#endif //_SCI_COLORMAPUSER_H_
