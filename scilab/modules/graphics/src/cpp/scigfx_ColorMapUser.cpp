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
/* file: scigfx_ColorMapUser.cpp                                              */
/* desc : implement the class ColorMapUser                                    */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#include "scigfx_ColorMapUser.h"


ColorMapUser::ColorMapUser(sciPointObj * pObj):
  sciGraphics::DrawableObject(pObj)
{
}

int ColorMapUser::sci2cppColorIndex(int colorindex)
{
  int m = getColorMapSize();

  if(colorindex<-2) colorindex = -2;
  if(colorindex>m+2) colorindex = m+2;

  //TODO allow index = -1
  if( colorindex ==  0 ) colorindex = 1;
  if( colorindex == -1 ) colorindex = m+1;
  if( colorindex == -2 ) colorindex = m+2;    

  return colorindex-1;
}

int ColorMapUser::getColorMapSize()
{
  return sciGetNumColors(m_pDrawed);
}

