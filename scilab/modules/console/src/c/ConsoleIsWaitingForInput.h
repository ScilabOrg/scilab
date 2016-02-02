/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __CONSOLEISWAITINGFORINPUT_H__
#define __CONSOLEISWAITINGFORINPUT_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_console.h"
#include "BOOL.h"

/**
* ConsoleIsWaitingForInput function
* @return true if the console is at the prompt
*/
CONSOLE_IMPEXP BOOL ConsoleIsWaitingForInput(void);

/*--------------------------------------------------------------------------*/
#endif /* __CONSOLEISWAITINGFORINPUT_H__ */

