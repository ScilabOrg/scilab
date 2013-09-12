
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

#ifndef __BASOUT_H__
#define __BASOUT_H__

#include "machine.h" /* C2F */
#include "dynlib_output_stream.h"

OUTPUT_STREAM_IMPEXP int C2F(basout)(int *io, int *lunit, char *string, long int nbcharacters);

#endif /* __BASOUT_H__ */
