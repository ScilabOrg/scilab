/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Compute automatic ticks
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "AutoLogSubticksComputer.hxx"

extern "C"
{
#include <string.h>
#include "Format.h"
#include "DrawObjects.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
AutoLogSubticksComputer::AutoLogSubticksComputer(DrawableSubwin * subwin)
  : ComputeSubticksStrategy(subwin)
{
  
}
/*------------------------------------------------------------------------------------------*/
AutoLogSubticksComputer::~AutoLogSubticksComputer(void)
{
  
}

/*------------------------------------------------------------------------------------------*/
int AutoLogSubticksComputer::getNbSubticks(double ticksPositions[], int nbTicks)
{
  return Max(0, ComputeNbSubTics(m_pDrawer->getDrawedObject(), nbTicks, 'l', ticksPositions, 0) * (nbTicks - 1));
}
/*------------------------------------------------------------------------------------------*/
void AutoLogSubticksComputer::getSubticksPosition(const double ticksPositions[], int nbTicks,
                                                    double subTickspositions[])
{
  // compute number of subtics
  int nbSubtics = ComputeNbSubTics(m_pDrawer->getDrawedObject(), nbTicks, 'l', ticksPositions, 0);

  /*    |              |              |    */
  /* ___|____|____|____|____|____|____|___ */
  /*   t0             t1             t2   */

  // draw only between two ticks, so skip last one
  for (int i = 0; i < nbTicks - 1; i++)
  {
    // decompose interval in nbsubtics parts
    double prevTick = ticksPositions[i];
    double nextTick = ticksPositions[i + 1];
    for (int j = 0; j < nbSubtics; j++)
    {
      // positions are given in logarithmic mode
      // so we need to switch ticks positions in normal mode
      // and then come back to log mode
      // the expression is then s = log(exp(t[i]) + (exp(t[i+1]) - exp(t[i])) * (j+1) / (nb+1).
      // A little cheat here. First I used the above formula to compute subticks position
      // which display a linear interpaolation between ticks (and transform to log during display).
      // Actually, this does not give good visual result if the interval between ticks
      // is not a 10 factor. When it is 10^2 or higher, the subticks are stuck to the right of the interval.
      // The cheat consists here to compute the discretisation of a an interval of size 10.
      // And then translate and move the result to obtain the final position
      double defaultIntervalPosition = log10(1.0 + 9.0 * (j + 1.0) / (nbSubtics + 1.0));

      // fit the result between prevTick and nextTick.
      subTickspositions[j + nbSubtics * i]
        = prevTick + (nextTick - prevTick) * defaultIntervalPosition;
    }
  }
}
/*------------------------------------------------------------------------------------------*/
}
