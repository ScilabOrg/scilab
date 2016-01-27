
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENSUBST_H__
#define __GENSUBST_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param n
 * @param dx
 * @param incx
 * @param dy
 * @param incy
 * @return
 */
INTEGER_IMPEXP int C2F(gensubst)(int *typ, int *n, int *dx, int *incx, int *dy, int *incy);

INTEGER_IMPEXP int C2F(gensubst1)(int *typ, int *n, int *dx, int *incx, int *dy, int *incy);

#endif /* __GENSUBST_H__ */
