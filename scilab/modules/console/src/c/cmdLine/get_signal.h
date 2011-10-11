/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#ifndef		GET_SIGNAL_H_
#define	GET_SIGNAL_H_

/**
 * Clear screen and get new terminfo,
 * This is a sighandler_t used when SIGWINCH is caught
 * @param signal caught.
 */
void getNewTerm(int signum);

void get_new_term(int signum);

#endif /* !SIGNAL_H_ */
