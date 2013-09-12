/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __CMP_H__
#define __CMP_H__

#include "machine.h"
#include "dynlib_elementary_functions.h"

ELEMENTARY_FUNCTIONS_IMPEXP int C2F(idcmp)(double x[], double y[], int *n, int res[], int *op);

#endif /* CMP_H__ */
