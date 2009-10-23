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
/* file: scigfx_Movable.h                                                     */
/* desc : declare the class Movable                                           */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#ifndef _SCI_MOVABLE_H_
#define _SCI_MOVABLE_H_

  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
#include "ObjectStructure.h"

  // C function about Movable
/* -------------------------------------------------------------------------- */
extern "C"
{
  int Move(sciPointObj * pObj, double displacement[], int displacementSize);
}
/* -------------------------------------------------------------------------- */

class Movable
{
public:
  virtual int move(double displacement[], int displacementSize)=0;
};

#endif //_SCI_MOVABLE_H_
