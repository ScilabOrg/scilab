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

#ifndef		CMD_FUNC_H_
#define	CMD_FUNC_H_

#include		"reader.h"

/**
 * Create and initialise a new link.
 * @param list of all commands.
 * @return pointer to new link.
 */
DoublyLinkedList *getNewCmd(DoublyLinkedList * lastCmd);

/**
 * Read standard input then call the good function.
 * @param list of all commands.
 * @param input stored.
 */
void getCmd(DoublyLinkedList ** listCmd);

/**
 * Free allocated links.
 * @param list of all commands.
 * @return left command line.
 */
DoublyLinkedList *freeCmd(DoublyLinkedList ** cmd);

/**
 * Delete links in the list of commands.
 * @param list of all commands.
 * @param number if links to keep before delete links.
 */
void deleteHistory(DoublyLinkedList * cmd, int limit);

/**
 * Save a command if listCmd is not NULL, else, display it.
 * @param list of command.
 */
void memCmd(DoublyLinkedList * listCmd, int cursorLocation);

#endif /* !CMD_FUNC_H_ */
