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
/* file: scigfx_PropertyContainer.cpp                                         */
/* desc : implement the methodes of PropertyContainer                         */
/*  //TODO étoffer                                                            */
/*----------------------------------------------------------------------------*/

#include "scigfx_PropertyContainer.h"

  // Include nécessaire au CastObject
/*----------------------------------------------------------------------------*/
#include "scigfx_CastObject.h"


    // C function about PropertyContainer
/* -------------------------------------------------------------------------- */
extern "C"
{
  int getPropertyByName(sciPointObj *pobj,char *marker)
  {
    return CastObject<PropertyContainer*>(pobj)->getPropertyByName(marker);
  }
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
  // Cette méthode essaye de récupérer la propriété en appellant la méthode 
  // protégée getPropertyByName(std::string) et prévient de l'inexistance de
  // la propriété.
int PropertyContainer::getPropertyByName(char* propertyName)
{
  // Convert char* to string.
  std::string pn(propertyName);
  // And ask to the property
  switch(getPropertyByName(pn))
  {
    case PROPERTY_RETURNED:
      return 0;
      break;
    case PROPERTY_NOT_FOUND:
      Scierror(999, _("'%s' property does not exist for this handle.\n"), propertyName) ;
      return -1;
      break;
    case GET_PROPERTY_ERROR:
      return -1;
      break;
  }
  return -1;
}
/*----------------------------------------------------------------------------*/
PropertyContainer::GetPropertyStatus PropertyContainer::aCStringForScilab(char* returnedCString)
{
  StrErr strErr;
  strErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &returnedCString);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return GET_PROPERTY_ERROR;
  }
  LhsVar(1) = Rhs + 1;
  return PROPERTY_RETURNED;
}
/*----------------------------------------------------------------------------*/
PropertyContainer::GetPropertyStatus PropertyContainer::aBooleanStringForScilab(bool value)
{
  char* on="on";
  char* off="off";
  StrErr strErr;
  strErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, value?&on:&off);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return GET_PROPERTY_ERROR;
  }
  LhsVar(1) = Rhs + 1;
  return PROPERTY_RETURNED;
}
/*----------------------------------------------------------------------------*/
PropertyContainer::GetPropertyStatus PropertyContainer::aDoubleForScilab(double value)
{
  StrErr strErr;
  strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &value);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return GET_PROPERTY_ERROR;
  }
  LhsVar(1) = Rhs + 1;
  return PROPERTY_RETURNED;
}
/*----------------------------------------------------------------------------*/

