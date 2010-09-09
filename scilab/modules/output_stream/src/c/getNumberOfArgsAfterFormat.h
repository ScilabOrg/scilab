/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETNUMBEROFARGSAFTERFORMAT_H__
#define __GETNUMBEROFARGSAFTERFORMAT_H__

#include <stdarg.h>

/**
* get numbers of arguments after format
* used by Scierror and sciprint
* @param[in] fmt a string 
* @param[in] args val_list
* @return number of argument after format
* -1 if fmt is NULL
*/
int getNumberOfArgsAfterFormat(const char *fmt, va_list args);

#endif /* __GETNUMBEROFARGSAFTERFORMAT_H__ */
