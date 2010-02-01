/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __WITH_PVM_H__
#define __WITH_PVM_H__

#include "dynlib_pvm.h"
#include "machine.h" /* C2F */
#include "BOOL.h"

/** 
* check if it is with PVM
* @param rep 1 (WITH) or 0 (WITHOUT)
* @return 0
*/
PVM_IMPEXP int C2F(withpvm)(int *rep);

/** 
* check if it is with PVM
* @return TRUE or FALSE
*/
PVM_IMPEXP BOOL withpvm(void);

#endif /* __WITH_PVM_H__ */
/*--------------------------------------------------------------------------*/ 
