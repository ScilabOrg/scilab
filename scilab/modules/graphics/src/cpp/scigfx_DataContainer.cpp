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
/* file: scigfx_DataContainer.cpp                                             */
/* desc : implement the class DataContainer                                   */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/


#include "scigfx_DataContainer.h"

  // Include nécessaire au CastObject
/*----------------------------------------------------------------------------*/
#include "scigfx_CastObject.h"


  // C function about DataContainer
/* -------------------------------------------------------------------------- */
extern "C"
{
  int SCIGFX_SetData(sciPointObj * pobj, double *tab, int *numrow, int *numcol)
  {
    return CastObject<DataContainer*>(pobj)->setData(tab, numrow, numcol);
  }
  void SCIGFX_GetData(sciPointObj * pobj, double *tab, int *numrow, int *numcol)
  {
    CastObject<DataContainer*>(pobj)->getData(tab, numrow, numcol);
  }
  int SCIGFX_GetDataSize(sciPointObj * pobj)
  {
    return CastObject<DataContainer*>(pobj)->getDataSize();
  }
}
/* -------------------------------------------------------------------------- */
