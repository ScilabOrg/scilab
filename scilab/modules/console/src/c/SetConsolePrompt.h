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

#ifndef __SETCONSOLEPROMPT_H__
#define __SETCONSOLEPROMPT_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_console.h"
#include "BOOL.h"
/**
* SetConsolePrompt function
* @param Sci_prompt new prompt to use for Scilab
* @return true ifthe prompt has been correcty set
*/
CONSOLE_IMPEXP BOOL SetConsolePrompt(char *Sci_prompt);

/*--------------------------------------------------------------------------*/
#endif /* __SETCONSOLEPROMPT_H__ */

