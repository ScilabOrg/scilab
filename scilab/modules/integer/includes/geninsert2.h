
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

#ifndef __GENINSERT2_H__
#define __GENINSERT2_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param mj
 * @param mi
 * @param indxj
 * @param indxi
 * @param to
 * @param mr
 * @param from
 * @param m3
 * @param inc3
 * @return
 */
INTEGER_IMPEXP int C2F(geninsert2)(int *typ, int *mj, int *mi, int *indxj, int *indxi, int *to, int *mr, int *from, int *m3, int *inc3);
#endif /* __GENINSERT2_H__ */
