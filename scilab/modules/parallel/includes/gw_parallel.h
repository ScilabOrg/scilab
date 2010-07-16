/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __GW_PARALLEL_H__
#define __GW_PARALLEL_H__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_parallel.h"
/*--------------------------------------------------------------------------*/ 
PARALLEL_IMPEXP int gw_parallel(void);
/*--------------------------------------------------------------------------*/ 
int sci_parallel_run(char * fname, int *_piKey);
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_PARALLEL_H__ */
/*--------------------------------------------------------------------------*/
