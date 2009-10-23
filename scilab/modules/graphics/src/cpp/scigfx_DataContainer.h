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
/* file: scigfx_DataContainer.h                                               */
/* desc : declare the class DataContainer                                     */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#ifndef _SCI_DATACONTAINER_H_
#define _SCI_DATACONTAINER_H_


  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
#include "ObjectStructure.h"

  // C function about DataContainer
/* -------------------------------------------------------------------------- */
extern "C"
{
  int  SCIGFX_SetData(sciPointObj * pObj, double *tab, int *numrow, int *numcol);
  void SCIGFX_GetData(sciPointObj * pObj, double *tab, int *numrow, int *numcol);
  int  SCIGFX_GetDataSize(sciPointObj * pObj);
}
/* -------------------------------------------------------------------------- */


  // "DataContainer" Class
/* -------------------------------------------------------------------------- */
class DataContainer
{
public:
  virtual int setData(double *tab, int *numrow, int *numcol)=0;
  virtual void getData(double *tab, int *numrow, int *numcol)=0;
  virtual int getDataSize()=0;
};
/* -------------------------------------------------------------------------- */

#endif //_SCI_DATACONTAINER_H_
