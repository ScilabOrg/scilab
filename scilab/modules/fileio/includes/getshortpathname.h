/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GETSHORTPATHNAME_H__
#define __GETSHORTPATHNAME_H__

#include "dynlib_fileio.h"
#include "machine.h" /* C2F */
#include "BOOL.h" /* BOOL */

/**
* Retrieves the short path form of the specified path
* @param[in] long path name
* @param[out] TRUE if conversion is ok
* @return short path name
* on Linux returns same path name
* used for Windows
*/
FILEIO_IMPEXP char *getshortpathname(char *longpathname, BOOL *convertok);

FILEIO_IMPEXP int C2F(getshortpathname)(char *pathname, int *len);

#endif /* __GETSHORTPATHNAME_H__ */
/*--------------------------------------------------------------------------*/
