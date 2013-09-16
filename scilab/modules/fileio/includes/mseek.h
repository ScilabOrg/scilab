/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __MSEEK_H__
#define __MSEEK_H__
#include "dynlib_fileio.h"
#include "machine.h"

/**
* seek function
*/
FILEIO_IMPEXP void C2F(mseek) (int *fd, double *offset, const char *flag, int *err);

#endif /* __MSEEK_H__ */
/*--------------------------------------------------------------------------*/
