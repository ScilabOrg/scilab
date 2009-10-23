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
/* file: scigfx_Visible.h                                                     */
/* desc : declare the class Visible                                           */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#ifndef _SCIGFX_VISIBLE_H_
#define _SCIGFX_VISIBLE_H_

  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
#include "ObjectStructure.h"

  // Include nécessaire pour l'héritage
/*----------------------------------------------------------------------------*/
#include "scigfx_PropertyContainer.h"


  // C function about Visible
/* -------------------------------------------------------------------------- */
extern "C"
{
  BOOL GetVisibility(sciPointObj * pObj);
  void SetVisibility(sciPointObj * pObj, BOOL value);
}



    // class Visible
/* -------------------------------------------------------------------------- */
class Visible:
  virtual public PropertyContainer
{
private:
  bool visible;
public:
  Visible();
  Visible(bool v);
  void setVisibility(bool v);
  bool getVisibility();

  // Implement PropertyContainer::getPropertyByName
  GetPropertyStatus getPropertyByName(const std::string& propertyName);
};


#endif //_SCIGFX_VISIBLE_H_
