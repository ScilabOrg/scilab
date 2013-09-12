
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENDIAG_H__
#define __GENDIAG_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param n
 * @param k
 * @param dx
 * @param dy
 * @return
 */
INTEGER_IMPEXP int C2F(gendiag)(int *typ, int *n, int *k, int *dx, int *dy);

#endif /* __GENDIAG_H__ */
