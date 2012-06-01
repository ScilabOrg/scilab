/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - scilab-Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_relocate_handle.h                                            */
/* desc : interface for relocate_handle routine                           */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "setGraphicObjectProperty.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "HandleManagement.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/

int sci_relocate_handle( char * fname, unsigned long fname_len )
{
  int handleCol ;
  int handleRow ;
  int nbHandle ;
  int handleStkIndex ;
  int parentCol ;
  int parentRow ;
  int parentStkIndex ;
  int outIndex ;
  int i ;

  long children = 0 ;
  char* childrenID = NULL;
  long parent = 0 ;
  char* parentID = NULL;

  /* the function should be called with relocate_handle( handle, parent_handle ) */
  CheckRhs(2,2) ;
  CheckLhs(0,1) ;


  GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &handleRow, &handleCol, &handleStkIndex );
  nbHandle = handleRow * handleCol ;
  GetRhsVar( 2,GRAPHICAL_HANDLE_DATATYPE, &parentRow, &parentCol, &parentStkIndex );

  if(parentCol * parentRow != 1)
  {
    Scierror(999,_("%s: Handles must be relocated under a single parent.\n"),fname);
    return 0 ;
  }

  parent = (long) *hstk(parentStkIndex);
  parentID = getObjectFromHandle(parent);

  for(i = 0; i < nbHandle; i++)
  {
    children = (long) *hstk( handleStkIndex + i );
    childrenID = getObjectFromHandle(children);
    setGraphicObjectRelationship(parentID, childrenID);
  }

  CreateVar( Rhs + 1,GRAPHICAL_HANDLE_DATATYPE, &handleCol, &handleRow, &outIndex );
  *hstk(outIndex) = *hstk(handleStkIndex) ;
  LhsVar(1) = Rhs + 1 ;
  PutLhsVar();
  return 0 ;

}
/*--------------------------------------------------------------------------*/
