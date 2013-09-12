/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GETCHARWITHOUTOUTPUT_H__
#define __GETCHARWITHOUTOUTPUT_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_console.h"
/*--------------------------------------------------------------------------*/
/**
* Make the console wait for a user input character (used for [more (y or n) ?])
* @return code for the character entered
*/
CONSOLE_IMPEXP int GetCharWithoutOutput(void);
/*--------------------------------------------------------------------------*/
#endif /* __GETCHARWITHOUTOUTPUT_H__ */

