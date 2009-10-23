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
/* file: scigfx_UserDataContainer.cpp                                         */
/* desc : implement the methodes of UserDataContainer                         */
/*  //TODO étoffer                                                            */
/*----------------------------------------------------------------------------*/

#include "scigfx_UserDataContainer.h"

  // Include nécessaire au CastObject
/*----------------------------------------------------------------------------*/
#include "scigfx_CastObject.h"


  // C function about UserDataContainer
/* -------------------------------------------------------------------------- */
extern "C"
{
  void GetPointerToUserData(sciPointObj * pobj,int ***user_data_ptr, int **size_ptr)
  {
      *user_data_ptr = &(CastObject<UserDataContainer*>(pobj)->user_data);
      *size_ptr =  &(CastObject<UserDataContainer*>(pobj)->size_of_user_data);
  }
}
/* -------------------------------------------------------------------------- */

UserDataContainer::UserDataContainer():
    user_data(0), size_of_user_data(0)
{
}


  // Implement PropertyContainer::getPropertyByName
PropertyContainer::GetPropertyStatus UserDataContainer::getPropertyByName(const std::string& propertyName)
{
    // TODO
    if(propertyName.compare("user_data")==0) return aCStringForScilab("TODO");
    return PROPERTY_NOT_FOUND;
}
