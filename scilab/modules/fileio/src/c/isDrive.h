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
#ifndef __ISDRIVE_H__
#define __ISDRIVE_H__

#include <wchar.h>

#include "BOOL.h" /* BOOL */

/**
* check if strname is a drive (only for windows)
* @param[in] a string
* @return TRUE or FALSE
* returns always false on linux
*/
BOOL isDrive(const char *strname);

/**
* check if strname is a drive (only for windows)
* @param[in] a wide string
* @return TRUE or FALSE
* returns always false on linux
*/

BOOL isDriveW(const wchar_t *wcstrname);

#endif
/*--------------------------------------------------------------------------*/
