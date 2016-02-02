
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
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENMCUPROD_H__
#define __GENMCUPROD_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param job
 * @param in
 * @param out
 * @param na
 * @param m
 * @param n
 * @return
 */
INTEGER_IMPEXP int C2F(genmcuprod)(int *typ, int *job, void * in, void * out, int * na, int * m, int * n);
INTEGER_IMPEXP int C2F(genmcuprodd)(int *typ, int *job, void * in, double * out, int * na, int * m, int * n);
#endif /* __GENMCUPROD_H__ */

