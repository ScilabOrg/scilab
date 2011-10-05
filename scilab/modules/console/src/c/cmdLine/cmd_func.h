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

/* TODO comment */
t_list_cmd *get_new_cmd(t_list_cmd * _last_cmd);

/* TODO comment */
void getCmd(t_list_cmd ** _list_cmd, int *_key);

/* TODO comment */
t_list_cmd *free_cmd(t_list_cmd ** cmd);

/* TODO comment */
void delteHistory(t_list_cmd * _cmd, int _limit);

/* TODO comment */
void mem_cmd(t_list_cmd * _list_cmd);

#endif /* !CMD_FUNC_H_ */
