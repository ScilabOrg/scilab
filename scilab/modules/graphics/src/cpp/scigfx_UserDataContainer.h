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
/* file: scigfx_UserDataContainer.h                                           */
/* desc : declare the class UserDataContainer                                 */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#ifndef _SCIGFX_USERDATACONTAINER_H_
#define _SCIGFX_USERDATACONTAINER_H_

  // Include nécessaire pour l'héritage
/*----------------------------------------------------------------------------*/
#include "scigfx_PropertyContainer.h"

  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
#include "ObjectStructure.h"



  // C function about UserDataContainer
/* -------------------------------------------------------------------------- */
extern "C"
{
  void GetPointerToUserData(sciPointObj * pObj,int ***user_data_ptr, int **size_ptr);
}
/* -------------------------------------------------------------------------- */

class UserDataContainer:
  virtual public PropertyContainer
{
public:
  UserDataContainer();
  int * user_data;
  int size_of_user_data;

  // Implement PropertyContainer::getPropertyByName
  GetPropertyStatus getPropertyByName(const std::string& propertyName);
};

#endif //_SCIGFX_USERDATACONTAINER_H_
