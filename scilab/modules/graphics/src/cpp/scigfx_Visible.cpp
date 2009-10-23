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
/* file: scigfx_Visible.cpp                                                   */
/* desc : implement the methodes of Visible                                   */
/*  //TODO étoffer                                                            */
/*----------------------------------------------------------------------------*/

#include "scigfx_Visible.h"

  // Include nécessaire au CastObject
/*----------------------------------------------------------------------------*/
#include "scigfx_CastObject.h"


/* -------------------------------------------------------------------------- */
// C function about Visible
extern "C"
{
  void SetVisibility(sciPointObj * pobj, BOOL value)
  {
    CastObject<Visible*>(pobj)->setVisibility(value!=0);
  }
  BOOL GetVisibility(sciPointObj * pobj)
  {
    return CastObject<Visible*>(pobj)->getVisibility()?TRUE:FALSE;
  }
}

/* -------------------------------------------------------------------------- */
// class Visible

Visible::Visible():
  visible(true)
{
}


Visible::Visible(bool v):
  visible(v)
{
}


void Visible::setVisibility(bool v)
{
  visible=v;
}

bool Visible::getVisibility()
{
  return visible;
}



  // Implement PropertyContainer::getPropertyByName
PropertyContainer::GetPropertyStatus Visible::getPropertyByName(const std::string& propertyName)
{
    if(propertyName.compare("visible")==0) return aBooleanStringForScilab(visible);
    return PROPERTY_NOT_FOUND;
}

