/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: CheckTicksProperty.h                                             */
/* desc : generic function to retrieve the assigned x_ticks, y_ticks or   */
/*        z_ticks property                                                */
/*------------------------------------------------------------------------*/

#ifndef _CHECK_TICKS_PROPERTY_C_
#define _CHECK_TICKS_PROPERTY_C_

#include "getPropertyAssignedValue.h"

/**
 * create a new assigne dlist which allow to easily retrieve the ticks
 * properties from the stack. Moreover the functgion checked if the list
 * contains the right properties.
 * @return the newly created list or NULL if an error occurred.
 */
AssignedList * createTlistForTicks(void* _pvCtx);

#endif /* _CHECK_TICKS_PROPERTY_C_ */
