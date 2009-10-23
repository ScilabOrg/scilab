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
/* file: scigfx_API.cpp                                                       */
/* desc : implement the SCIGFX API functions                                  */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#include "scigfx_API.h"

  // Include nécessaire au CastObject
/*----------------------------------------------------------------------------*/
#include "scigfx_CastObject.h"

  // C function for SCIGFX API
/* -------------------------------------------------------------------------- */
extern "C"
{
  void DeleteFeatures(sciPointObj * pObj)
  {
    delete(pObj->pfeatures);
    pObj->pfeatures=0;
  }

  void SetHimselfAsDrawer(sciPointObj * pObj)
  {
    sciGraphics::DrawableObjectWrapper* wrapper = new sciGraphics::DrawableObjectWrapper();
    wrapper->drawer = CastObject<sciGraphics::DrawableObject*>(pObj);
    pObj->pDrawer = wrapper;
  }
}
/* -------------------------------------------------------------------------- */
