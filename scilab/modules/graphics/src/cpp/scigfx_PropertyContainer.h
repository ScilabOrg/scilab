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
/* file: scigfx_PropertyContainer.h                                           */
/* desc : declare the class PropertyContainer                                 */
/*  //TODO traduire                                                           */
/*  Cette classe contient les méthodes nécessaire au renvoi d'une valeur      */
/*  a scilab en fonction du nom de la propriété souhaitée                     */
/*----------------------------------------------------------------------------*/

#ifndef _SCIGFX_PROPERTYCONTAINER_H_
#define _SCIGFX_PROPERTYCONTAINER_H_

  // Include nécessaire pour getPropertyByName
/*----------------------------------------------------------------------------*/
#include <string>

  // Include nécessaire au retour de variables dans scilab.
/*----------------------------------------------------------------------------*/
#include "stack-c.h"
#include "api_scilab.h"
#include "Scierror.h"

  // Include nécessaire pour la localisation ie '_'
/*----------------------------------------------------------------------------*/
#include "localization.h"

  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
#include "ObjectStructure.h"

    // C function about PropertyContainer
/* -------------------------------------------------------------------------- */
extern "C"
{
  int GetPropertyByName(sciPointObj *pobj,char *marker);
}
/* -------------------------------------------------------------------------- */

class PropertyContainer
{
protected:
  // etat possible du retour
  enum GetPropertyStatus { PROPERTY_RETURNED, PROPERTY_NOT_FOUND, GET_PROPERTY_ERROR };

  // fonction que doivent implémenter les classes héritantes.
  virtual GetPropertyStatus getPropertyByName(const std::string& propertyName)=0;

  // Méthodes facilitant le retour de variables.
static GetPropertyStatus aCStringForScilab(char* returnedCString);
static GetPropertyStatus aBooleanStringForScilab(bool value);
static GetPropertyStatus aDoubleForScilab(double value);

public:
  // fonction appellé de l'extérieur.
  int getPropertyByName(char* propertyName);
};


#endif //_SCIGFX_PROPERTYCONTAINER_H_
