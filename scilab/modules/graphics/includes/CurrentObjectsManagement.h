/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: CurrentObjectsManagement.h                                       */
/* desc : Set of functions to set and get current objects                 */
/*------------------------------------------------------------------------*/

#ifndef _GET_CURRENT_OBJECTS_H_
#define _GET_CURRENT_OBJECTS_H_

#include "ObjectStructure.h"
#include "BOOL.h"
/**
 * Returns the pointer to the current selected figure. 
 */
sciPointObj * sciGetCurrentFigure( void ) ;

/**
 * To know if a figure is the selected one.
 */
BOOL sciIsCurrentFigure(sciPointObj * pFigure);

/**
 * Modify the current figure pointeR.
 */
int sciInitCurrentFigure( sciPointObj * mafigure ) ;

/**
 * Modify the current figure if needed.
 */
int sciSetCurrentFigure( sciPointObj * mafigure ) ;
/*-----------------------------------------------------------------------------*/
/**
 * Get the current Object
 */
sciPointObj * sciGetCurrentObj( void ) ;

/**
 * Modify the current object pointer.
 */
void sciSetCurrentObj( sciPointObj * pobj ) ;

/**
 * @return TRUE if the object is the current one, false otherwise
 */
BOOL sciIsCurrentObject(sciPointObj * pobj);

/**
 * Return the handle on the current object
 */
long sciGetCurrentHandle( void ) ;
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentSubWin( void ) ;
/*-----------------------------------------------------------------------------*/

#endif /* _GET_CURRENT_OBJECTS_H_ */
