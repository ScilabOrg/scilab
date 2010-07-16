/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_user_data_property.c                                         */
/* desc : function to retrieve in Scilab the user_data field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_user_data_property( sciPointObj * pobj )
{
  /* user_data */
  int userDataSize;
  int* userData;

  userDataSize = *(int*)getGraphicObjectProperty(pobj->UID, __GO_USER_DATA_SIZE__, jni_int);

  userData = (int*)getGraphicObjectProperty(pobj->UID, __GO_USER_DATA__, jni_int_vector);

  if ( userData == NULL || userDataSize == 0 )
  {
    return sciReturnEmptyMatrix() ;
  }
  else
  {
    return sciReturnUserData( userData, userDataSize);
  }
}
/*------------------------------------------------------------------------*/
