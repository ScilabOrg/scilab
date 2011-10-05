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

#ifndef		GOTO_WORDS_H_
#define	GOTO_WORDS_H_

#include	"reader.h"

/**
 * Move cursor to the beginning of the next word or to the end of the string if none were found.
 * @param current edited line
 * @param user Input
 * @return user Input
 */
int nextWord(t_list_cmd ** _cmd, int _key);

/**
 * Move cursor to the beginning of the previous word or to the beginning of the string if none were found.
 * @param current edited line
 * @param user Input
 * @return user Input
 */
int previousWord(t_list_cmd ** _cmd, int _key);

#endif /* !GOTO_WORDS_H_ */
