/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
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
/* file: set_z_ticks_property.c                                           */
/* desc : function to modify in Scilab the z_ticks field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include <math.h>
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "CheckTicksProperty.h"
#include "MALLOC.h"
#include "BasicAlgos.h"
#include "DrawObjects.h"
#include "freeArrayOfString.h"
#include "loadTextRenderingAPI.h"

/*------------------------------------------------------------------------*/
/* @TODO: remove stackPointer, nbRow, nbCol which are used */
int set_z_ticks_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  AssignedList * tlist     = NULL ;
  sciSubWindow * ppSubWin  = NULL ;
  int            nbTicsRow = 0    ;
  int            nbTicsCol = 0    ;

  if ( !isParameterTlist( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Typed list expected.\n"), "z_ticks");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"z_ticks");
    return SET_PROPERTY_ERROR ;
  }

  ppSubWin = pSUBWIN_FEATURE(pobj) ;

  tlist = createTlistForTicks() ;

  if ( tlist == NULL )
  {
    return SET_PROPERTY_ERROR ;
  }

  /* locations */
  FREE( ppSubWin->axes.u_zgrads ) ;
  ppSubWin->axes.u_zgrads = NULL ;

  destroyStringArray( ppSubWin->axes.u_zlabels, ppSubWin->axes.u_nzgrads ) ;
  ppSubWin->axes.u_zlabels = NULL ;

  ppSubWin->axes.u_nzgrads = 0 ;

  ppSubWin->axes.u_zgrads = createCopyDoubleMatrixFromList( tlist, &nbTicsRow, &nbTicsCol ) ;

  if ( ppSubWin->axes.u_zgrads == NULL && nbTicsRow == -1 )
  {
    // if nbTicsRow = 0, it's just an empty matrix
    Scierror(999, _("%s: No more memory.\n"),"set_z_ticks_property");
    return SET_PROPERTY_ERROR ;
  }

  if ( ppSubWin->logflags[2] == 'l' )
  {
    int  i ;
    for ( i = 0 ; i < nbTicsCol * nbTicsCol ; i++ )
    {
      ppSubWin->axes.u_zgrads[i] = log10( ppSubWin->axes.u_zgrads[i] ) ;
    }
  }
  else
  {
    /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    ppSubWin->axes.nbsubtics[2] = ComputeNbSubTics( pobj,ppSubWin->axes.u_nzgrads,'n',NULL,ppSubWin->axes.nbsubtics[2] ) ;
  }

  /*  labels */
  // Here we check the size of "locations" instead of "labels", but they have the same size.
  // We need to check the size to not be 0 because an empty matrix is a matrix of double
  // and 'getCurrentStringMatrixFromList' expect a matrix of string (see bug 5148).
  // P.Lando
  if( nbTicsCol * nbTicsRow )
  {
    ppSubWin->axes.u_zlabels = getCurrentStringMatrixFromList( tlist, &nbTicsRow, &nbTicsCol );
    /* Check if we should load LaTex / MathML Java libraries */
    loadTextRenderingAPI(ppSubWin->axes.u_zlabels, nbTicsCol, nbTicsRow);
  }
  else
  {
    ppSubWin->axes.u_zlabels = NULL;
  }


  ppSubWin->axes.u_nzgrads = nbTicsRow * nbTicsCol ;
  ppSubWin->axes.auto_ticks[2] = FALSE ;

  destroyAssignedList( tlist ) ;

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
