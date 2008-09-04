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
/* file: set_tics_labels_property.c                                       */
/* desc : function to modify in Scilab the tics_labels field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int set_tics_labels_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"tics_labels") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"tics_labels") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow != 1)
  {
    sciprint(_("%s: Wrong type for second input argument: row vector expected.\n"), "set_tics_labels_property");
    return SET_PROPERTY_ERROR ;
  }

  if ( pAXES_FEATURE(pobj)->nb_tics_labels > nbCol )
  {
    sciprint("Value must have at least %d elements",pAXES_FEATURE(pobj)->nb_tics_labels) ;
    return SET_PROPERTY_ERROR ;
  }

  if(pAXES_FEATURE(pobj)->str != NULL)
  {
    destroyStringArray( pAXES_FEATURE(pobj)->str, pAXES_FEATURE(pobj)->nb_tics_labels ) ;
  }

  pAXES_FEATURE(pobj)->str = createCopyStringMatrixFromStack( stackPointer, nbCol ) ;
  pAXES_FEATURE(pobj)->nb_tics_labels = nbCol ; /* could be increased to support xy_type switching (i.e. xy_type='v' -> xy_type='r') */

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
