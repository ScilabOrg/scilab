/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_swap_handles.c                                               */
/* desc : interface for swap_handles routine                              */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "HandleManagement.h"
#include "localization.h"
#include "Scierror.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
int sci_swap_handles( char * fname, unsigned long fname_len )
{
  int firstHdlCol  ;
  int firstHdlRow  ;
  int secondHdlCol ;
  int secondHdlRow ;
  int firstHdlStkIndex  ;
  int secondHdlStkIndex ;
  char *pstHandle_1;
  char *pstHandle_2;
  char *pstParent_1;
  char *pstParent_2;
  int iChildrenCount = 0;
  int *piChildrenCount = &iChildrenCount;
  char **pstChildrenUID;
  int i = 0;

  CheckRhs( 2, 2 ) ;
  CheckLhs( 0, 1 ) ;

  GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &firstHdlRow, &firstHdlCol, &firstHdlStkIndex );
  GetRhsVar( 2,GRAPHICAL_HANDLE_DATATYPE, &secondHdlRow, &secondHdlCol, &secondHdlStkIndex );

  if ( firstHdlRow * firstHdlCol != 1 || secondHdlRow * secondHdlCol != 1 )
  {
    Scierror(999,_("%s: Routine can only swap two single handles.\n"),fname);
    return 0 ;
  }

  /* get the two handles and swap them */
  pstHandle_1 = getObjectFromHandle(*hstk(firstHdlStkIndex));
  pstHandle_2 = getObjectFromHandle(*hstk(secondHdlStkIndex));
  getGraphicObjectProperty(pstHandle_1, __GO_PARENT__, jni_string, &pstParent_1);
  getGraphicObjectProperty(pstHandle_2, __GO_PARENT__, jni_string, &pstParent_2);

  // Check if objects do not have the same parent
  if (strcmp(pstParent_1, pstParent_2) == 0)
  {
      getGraphicObjectProperty(pstParent_1, __GO_CHILDREN_COUNT__, jni_int, (void **)&piChildrenCount);
      getGraphicObjectProperty(pstParent_1, __GO_CHILDREN__, jni_string_vector, (void **)&pstChildrenUID);

      for (i = 0 ; i < iChildrenCount ; ++i)
      {
          if (strcmp(pstChildrenUID[i], pstHandle_1) == 0)
          {
              pstChildrenUID[i] = pstHandle_2;
          }
          else if (strcmp(pstChildrenUID[i], pstHandle_2) == 0)
          {
              pstChildrenUID[i] = pstHandle_1;
          }
      }

      setGraphicObjectProperty(pstParent_1, __GO_CHILDREN__, pstChildrenUID, jni_string_vector, iChildrenCount);

  }
  else
  {
      setGraphicObjectRelationship(pstParent_1, pstHandle_2);
      setGraphicObjectRelationship(pstParent_2, pstHandle_1);
  }
  LhsVar(1) = 0 ;
  PutLhsVar();
  return 0 ;
}
/*--------------------------------------------------------------------------*/
