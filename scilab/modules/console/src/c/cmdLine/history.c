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

#include	<stdio.h>
#include	<unistd.h>
#include	<term.h>
#include	<termios.h>
#include	"cap_func.h"
#include	"goto_func.h"
#include	"aff_prompt.h"
#include	"reader.h"

/* Get the previous command line */
int previousCmd(t_list_cmd ** _cmd, int _key)
{
    int promptSize;

    _key = 0;
    if ((*_cmd)->previous)
    {
        /* Go the beginning of the current edited line then clearn the screen from */
        while ((*_cmd)->index)
        {
            gotoLeft(_cmd, _key);
        }
        capStr("up");
        capStr("do");
        capStr("cd");
        /* Get the new command line then display it */
        promptSize = getPrompt(WRT_PRT);
        (*_cmd) = (*_cmd)->previous;
        printf("%ls", (*_cmd)->cmd);
        fflush(stdout);
        (*_cmd)->index = (*_cmd)->line;
        /*
         * if the last character is on the last column of the window,
         * put the cursor on the first column of the next line.
         *
         * Must be done, else the cursor disappear and bug.
         */
        if (!(((*_cmd)->index + promptSize) % tgetnum("co")))
        {
            capStr("do");
        }
    }
    return (_key);
}

/* Get the next command line */
int nextCmd(t_list_cmd ** _cmd, int _key)
{
    int promptSize;

    _key = 0;
    if ((*_cmd)->next)
    {
        /* Go the beginning of the current edited line then clearn the screen from */
        while ((*_cmd)->index)
        {
            gotoLeft(_cmd, _key);
        }
        capStr("up");
        capStr("do");
        capStr("cd");
        /* Get the new command line then display it */
        (*_cmd) = (*_cmd)->next;
        promptSize = getPrompt(WRT_PRT);
        printf("%ls", (*_cmd)->cmd);
        fflush(stdout);
        (*_cmd)->index = (*_cmd)->line;
        /*
         * if the last character is on the last column of the window,
         * put the cursor on the first column of the next line.
         *
         * Must be done, else the cursor disappear and bug.
         */
        if (!(((*_cmd)->index + promptSize) % tgetnum("co")))
        {
            capStr("do");
        }
    }
    return (_key);
}
