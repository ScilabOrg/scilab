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

#include		<wchar.h>
#include		<wctype.h>
#include		<stdlib.h>
#include		<stdio.h>
#include		<unistd.h>
#include		<curses.h>
#include		<termios.h>
#include		<term.h>
#include		"history.h"
#include		"reader.h"
#include		"cap_func.h"
#include		"goto_func.h"
#include		"charctl.h"
#include		"init_tc_shell.h"
#include		"aff_prompt.h"
#include		"goto_words.h"

int caseHomeOrEndKey(t_list_cmd ** _listCmd)
{
    switch (getwchar())
    {
    case L'H':
        return (begLine(_listCmd, 0));
    case L'F':
        return (endLine(_listCmd, 0));
    default:
        printf("\a");
        fflush(stdout);
        return (0);
    }
}

/*
 * If second key was L'['
 * It mean this an arrow key or delete key.
 */

int caseDelOrArrowKey(t_list_cmd ** _listCmd)
{
    switch (getwchar())
    {
    case L'A':
        return (previousCmd(_listCmd, 0));
    case L'B':
        return (nextCmd(_listCmd, 0));
    case L'C':
        return (gotoRight(_listCmd, 0));
    case L'D':
        return (gotoLeft(_listCmd, 0));
    case L'3':
        if (getwchar() == L'~')
            return (rmLetter(_listCmd, SCI_DELETE));
    default:
        printf("\a");
        fflush(stdout);
        return (0);
    }
}

/*
 * If last key was Meta...
 */
int caseMetaKey(t_list_cmd ** _listCmd)
{
    switch (getwchar())
    {
    case L'f':
    case L'F':
        return (nextWord(_listCmd, 0));
    case L'b':
    case L'B':
        return (previousWord(_listCmd, 0));
    case L'[':
        return (caseDelOrArrowKey(_listCmd));
    case L'O':
        return (caseHomeOrEndKey(_listCmd));
    default:
        printf("\a");
        fflush(stdout);
        return (0);
    }
}

/*
 * Read keyboard a first time.
 */

int getKey(t_list_cmd ** _listCmd)
{
    int key;

    key = getwchar();
    switch (key)
    {
    case CTRL_A:
        return (begLine(_listCmd, key));
    case CTRL_B:
        return (gotoLeft(_listCmd, key));
    case CTRL_E:
        return (endLine(_listCmd, key));
    case CTRL_F:
        return (gotoRight(_listCmd, key));
    case CTRL_K:
        return (deleteLineFromCurs(_listCmd, key));
    case CTRL_N:
        return (nextCmd(_listCmd, key));
    case CTRL_P:
        return (previousCmd(_listCmd, key));
    case ESCAPE:
        return (caseMetaKey(_listCmd));
    case SCI_BACKSPACE:
        return (rmLetter(_listCmd, SCI_BACKSPACE));
    default:
        if (key == L'\n')
            return ('\n');
        return (addLetter(_listCmd, key));
    }
}

/*
 * If there is a string the function save it.
 * else The function write the saved str.
 */
void mem_cmd(t_list_cmd ** _cmd)
{
    static t_list_cmd **mem_list;
    static int i;

    if (_cmd != NULL)
    {
        mem_list = _cmd;
        i = (*_cmd)->index;
    }
    else
    {
/* TODO comment */
        i = (*mem_list)->index;
        (*mem_list)->index = (*mem_list)->line;
        printf("%ls", (*mem_list)->cmd);
/* TODO probably useless. We are doing a buffering word by word */
        fflush(stdout);
        while ((*mem_list)->index != i)
	{
            gotoLeft(mem_list, 0);
	}
    }
}

t_list_cmd *getNewCmd(t_list_cmd * _lastCmd)
{
    t_list_cmd *newCmd;

    newCmd = MALLOC(sizeof(*newCmd));
    if (_lastCmd != NULL)
    {
        _lastCmd->next = newCmd;
        _lastCmd->bin = 1;
    }
    newCmd->previous = _lastCmd;
    newCmd->next = NULL;
    newCmd->line = 0;
    newCmd->bin = 0;
    newCmd->nbr_line = 0;
    return (newCmd);
}

void getCmd(t_list_cmd ** _listCmd, int *_key)
{
    int bin;

    (*_listCmd)->index = 0;
    *_key = 0;
    bin = 1;
    while (bin)
    {
        mem_cmd(_listCmd);
        if (getKey(_listCmd) == '\n')
        {
            putchar('\n');
            bin = 0;
        }
    }
}

t_list_cmd *free_cmd(t_list_cmd ** _cmd)
{
    t_list_cmd *save;

    save = *_cmd;
    *_cmd = (*_cmd)->previous;
    free(save->cmd);
    free(save);
    if (*_cmd)
        (*_cmd)->next = NULL;
    return (*_cmd);
}

void deleteHistory(t_list_cmd * _cmd, int _limit)
{
    while (_limit && _cmd->previous)
    {
        _cmd = _cmd->previous;
        _limit--;
    }
    if (!_limit)
    {
        _cmd = _cmd->next;
        free(_cmd->previous->cmd);
        free(_cmd->previous);
        _cmd->previous = NULL;
    }
}
