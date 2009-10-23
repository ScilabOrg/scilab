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
/* file: scigfx_Movable.cpp                                                   */
/* desc : implement the class Movable                                         */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#include "scigfx_Movable.h"

  // Include nécessaire au CastObject
/*----------------------------------------------------------------------------*/
#include "scigfx_CastObject.h"

  // C function about Movable
/* -------------------------------------------------------------------------- */
extern "C"
{
  int Move(sciPointObj * pobj, double displacement[], int displacementSize)
  {
    return CastObject<Movable*>(pobj)->move(displacement, displacementSize);
  }
}
/* -------------------------------------------------------------------------- */
