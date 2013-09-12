/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __BACKTRACE_PRINT_H__
#define __BACKTRACE_PRINT_H__

/*
 * Build a backtrace.
 * @param first_level stack level where to begin to print
 * @param unmangle if !=0 then demangle the functions names
 * @return a string (to be freed) containing the complete backtrace
 */
const char * backtrace_print(int first_level, int unmangle);

#endif // __BACKTRACE_PRINT_H__
