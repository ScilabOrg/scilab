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

#ifndef		HISTORY_H_
#define	HISTORY_H_

#include "reader.h"
#include "history.h"

/**
 * Change current editing line to prev editing line
 * @param list of all command lines.
 * @param cursor location in the command line.
 * @return user input
 */
int previousCmd(DoublyLinkedList ** cmd, unsigned int *cursorLocation);

/**
 * Change current editing line to next editing line
 * @param list of all command lines.
 * @param cursor location in the command line.
 * @return user input
 */
int nextCmd(DoublyLinkedList ** cmd, unsigned int *cursorLocation);

#endif /* !HISTORY_H_ */
