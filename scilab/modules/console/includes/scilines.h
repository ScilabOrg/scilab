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
#ifndef __SCILINES_H__
#define __SCILINES_H__

#include "machine.h" /* C2F */
#include "BOOL.h"
#include "dynlib_console.h"

/**
* set number of lines and columns to display data
* @param[in] number of lines
* @param[in] number of columns
* @return 0
*/
CONSOLE_IMPEXP int scilines(int nl, int nc);

/**
* set default number of lines and columns to display data
* @return 0
*/
CONSOLE_IMPEXP int scilinesdefault(void);

/**
* set number of lines and columns to display data (C2F version)
* @param[in] number of lines
* @param[in] number of columns
* @return 0
*/
CONSOLE_IMPEXP int C2F(scilines)(int *nl, int *nc);

/**
* set default number of lines and columns to display data (C2F version)
* @return 0
*/
CONSOLE_IMPEXP int C2F(scilinesdefault)(void);

/**
* set number of columns to display data
* @param new size of columns
* @return TRUE or FALSE
*/
CONSOLE_IMPEXP BOOL setColumnsSize(int colums);

/**
* set number of lines to display data
* @param new size of lines
* @return TRUE or FALSE
*/
CONSOLE_IMPEXP BOOL setLinesSize(int lines_);

/**
* get number of columns to display data
* @return numbers of columns
*/
CONSOLE_IMPEXP int getColumnsSize(void);

/**
* get number of lines to display data
* @return numbers of lines
*/
CONSOLE_IMPEXP int getLinesSize(void);

#endif /* __SCILINES_H__ */
/*--------------------------------------------------------------------------*/
