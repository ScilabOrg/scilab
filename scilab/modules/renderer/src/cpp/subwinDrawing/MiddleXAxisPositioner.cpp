/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing ticks  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "MiddleXAxisPositioner.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
MiddleXAxisPositioner::MiddleXAxisPositioner(DrawableSubwin * subwin)
  : XAxisPositioner(subwin)
{

}
/*------------------------------------------------------------------------------------------*/
MiddleXAxisPositioner::~MiddleXAxisPositioner(void)
{

}
/*------------------------------------------------------------------------------------------*/
void MiddleXAxisPositioner::getAxisBounds(double startBound[3], double endBound[3])
{
  double zCoordinate = findUpperZCoordinate();
  double yCoordinate = findMiddleYCoordinate(zCoordinate);

  startBound[0] = m_dXmin;
  startBound[1] = yCoordinate;
  startBound[2] = zCoordinate;

  endBound[0] = m_dXmax;
  endBound[1] = yCoordinate;
  endBound[2] = zCoordinate;
}
/*------------------------------------------------------------------------------------------*/
double MiddleXAxisPositioner::findMiddleYCoordinate(double zCoordinate)
{
  // find if 0 is in the range of Y coordinates
  // if so then one of yMax or yMax is <= 0 and the other is not
  if (m_dYmin * m_dYmax <= 0.0)
  {
    return 0.0;
  }
  else if (m_dYmax < 0.0)
  {
  // both bounds are negative
  return m_dYmax;
  }
  else
  {
    // both bounds are positive
    return m_dYmin;
  }
}
/*------------------------------------------------------------------------------------------*/

}
