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
#ifndef __GENSUM_H__
#define __GENSUM_H__
#include "machine.h"
#include "dynlib_integer.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param n
 * @param dx be carreful using this parameter (crappy way of doing polymorphism)
 * @param incx
 * @return
 */
INTEGER_IMPEXP int C2F(gensum)(int *typ, int *n, void *dx, int *incx);
INTEGER_IMPEXP double C2F(gensumasdouble)(int *typ, int *n, void *dx, int *incx);
#endif /* __GENSUM_H__ */
