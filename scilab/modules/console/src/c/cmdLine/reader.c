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
#include		<signal.h>
#include                "MALLOC.h"
#include		"cmd_func.h"
#include		"history.h"
#include		"reader.h"
#include		"cap_func.h"
#include		"goto_func.h"
#include		"charctl.h"
#include		"init_tc_shell.h"
#include		"aff_prompt.h"
#include		"get_signal.h"
#include		"charEncoding.h"

/* comment */
wchar_t *cmdDup(t_list_cmd * _cmd, wchar_t * _wcs)
{
    wchar_t *dupCmd;

    /* TODO: Document */
    dupCmd = MALLOC(sizeof(*dupCmd) * (_cmd->line + 1));
    dupCmd[_cmd->line] = L'\0';
    /*
     * Get the number of line the command line has:
     * Add the size of the prompt and the size of the command line
     * then divid it by the number of column in the windows.
     */
    _cmd->nbr_line = 1 + (_cmd->line + getPrompt(NOWRT_PRT)) / tgetnum("co");
    if (_wcs != NULL)
    {
        wcscpy(dupCmd, _wcs);
    }
    else
    {
        wcscpy(dupCmd, _cmd->cmd);
    }
    return (dupCmd);
}

/* Create a new link for the history. */
t_list_cmd *getNewHist(t_list_cmd * _cmd)
{
    t_list_cmd *lastCmd;

    lastCmd = _cmd;
    while (lastCmd->bin)
    {
        lastCmd = lastCmd->next;
    }
    lastCmd->line = _cmd->line;
    lastCmd->nbr_line = _cmd->nbr_line;
    lastCmd->cmd = cmdDup(_cmd, NULL);
    return (lastCmd);
}

/*
 * Take all spaces ans tabulations at the beginning of the command line out.
 * return the cleaned command line.
 */
void *cleanVoidCharInCmd(t_list_cmd * _cmd)
{
    int i;
    wchar_t *dupCmd;

    i = 0;
    /* Passing all void character at the beginning of the command line... */
    while (_cmd->cmd[i] && (_cmd->cmd[i] == L' ' || _cmd->cmd[i] == L'\t'))
    {
        i++;
    }
    /* ... then create the new command line. */
    _cmd->line = wcslen(&_cmd->cmd[i]);
    dupCmd = cmdDup(_cmd, &_cmd->cmd[i]);
    free(_cmd->cmd);
    _cmd->cmd = dupCmd;
    return (dupCmd);
}

/*
 * Initialise current line edited
 * Return the line sent by user
 */
t_list_cmd *initUsrInput(t_list_cmd * _listCmd)
{
    int key;
    int ret;

    _listCmd = getNewCmd(_listCmd);
    getPrompt(WRT_PRT);
    key = 0;
    _listCmd->line = 0;
/* Hardcoded value */
    _listCmd->cmd = MALLOC(sizeof(*_listCmd->cmd) * 1024);
    _listCmd->cmd[0] = L'\0';
    ret = 0;
/* please comment */
    getCmd(&_listCmd, &key);
    if (key == CTRL_D)
        return (NULL);
    cleanVoidCharInCmd(_listCmd);
    if (_listCmd->bin)
    {
        _listCmd = getNewHist(_listCmd);
    }
    deleteHistory(_listCmd, 200);
    return (_listCmd);
}

/*
 * Get final command line, then convert wchar to multi-byte char
 * return a string or NULL in case there's an error.
 */
char *getCmdLine(t_list_cmd ** history)
{
    char *dest;

    /* TODO: Catch SIGINT */
    signal(SIGWINCH, getNewTerm);
    *history = initUsrInput(*history);
    if (*history == NULL)
        return (NULL);
    dest = strdup(wide_string_to_UTF8((*history)->cmd));
    return (dest);
}
