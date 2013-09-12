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
#ifndef __MPUT_H__
#define __MPUT_H__

#include <stdio.h>
#include "dynlib_fileio.h"
#include "machine.h"

/**
* mput functions  write data in a machine
* independant way (i.e write in little-endian)
* or can be used to write in little or big endian
*/
FILEIO_IMPEXP void C2F(mput) (int *fd, double *res, int *n, char *type, int *ierr);

FILEIO_IMPEXP void C2F(mputnc) (int *fd, void * res, int *n1, char *type, int *ierr);

FILEIO_IMPEXP void mput2 (FILE *fa, int swap2, double *res, int n, char *type, int *ierr);

#endif /* __MPUT_H__ */
/*--------------------------------------------------------------------------*/
