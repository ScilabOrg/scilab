/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CHECKFORMAT_HXX__
#define __CHECKFORMAT_HXX__

#include "dynlib_fileio.h"
#include "internal.hxx"

#define CHECK_ERROR_BAD_FORMAT -1
#define CHECK_ERROR_INCOMPLETE_FORMAT -2

int checkFormat(wchar_t* _wcsFormat, unsigned int* _cFormat);

#endif /* !__CHECKFORMAT_HXX__ */
