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
/* file: scigfx_CastObject.h                                                  */
/* desc : declare the function CastObject                                     */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#ifndef _SCIGFX_CASTOBJECT_H_
#define _SCIGFX_CASTOBJECT_H_

  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
#include "ObjectStructure.h"

  // Déclaration nécessaire pour les cast
/*----------------------------------------------------------------------------*/
#include "scigfx_Rectangle.h"

  // Include nécessaire pour sciGetEntityType
/*----------------------------------------------------------------------------*/
#include "GetProperty.h"

template<typename T> T CastObject(sciPointObj* pobj)
{
  switch (sciGetEntityType (pobj))
  {
    case SCI_RECTANGLE:
      return reinterpret_cast<Rectangle*>(pobj->pfeatures);
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC:
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_LEGEND:
    case SCI_TEXT:
    case SCI_LABEL:
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
      return 0;
      break;
  }
  return 0;
}

#endif //_SCIGFX_CASTOBJECT_H_
