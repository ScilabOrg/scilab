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
/* file: scigfx_API.h                                                         */
/* desc : declare the SCIGFX API functions                                    */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#ifndef _SCI_API_H_
#define _SCI_API_H_

  // Include nécessaire pour DrawableObjectWrapper
/*----------------------------------------------------------------------------*/
#include "getHandleDrawer.h"

extern "C"
{
  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
  #include "ObjectStructure.h"
}

  // C function for SCIGFX API
/* -------------------------------------------------------------------------- */
extern "C"
{
  void DeleteFeatures(sciPointObj * pObj);
  void SetHimselfAsDrawer(sciPointObj * pObj);
}
/* -------------------------------------------------------------------------- */


#endif //_SCI_API_H_
