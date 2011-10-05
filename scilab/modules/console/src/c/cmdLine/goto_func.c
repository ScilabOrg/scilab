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

#include	<termios.h>
#include	<curses.h>
#include	<term.h>
#include	<string.h>
#include	"reader.h"
#include	"cap_func.h"
#include	"aff_prompt.h"

/* Move cursor to the right */
int gotoRight(t_list_cmd ** _cmd, int _key)
{
    int nbrCol;
    int promptSize;

    promptSize = getPrompt(NOWRT_PRT);
    _key = 0;
    nbrCol = tgetnum("co");
    /* if the cursor is not at the end of the command line */
    if ((*_cmd)->index != (*_cmd)->line)
    {
        /* If the cursor is on the last column... */
        if (!(((*_cmd)->index + promptSize + 1) % nbrCol))
        {
            /* move the cursor down. */
            capStr("do");
        }
        else
        {
            /* else, move it to the right */
            capStr("nd");
        }
        ((*_cmd)->index)++;
    }
    /* else, if the cursor is next to the last column of the window, move it down a line */
    else if (!(((*_cmd)->index + promptSize) % nbrCol))
    {
        capStr("do");
    }
    return (_key);
}

/* go to the last column of the current line */
void findEndLine(int _i, int _promptSize, int _nbrCol)
{
    if (!((_i + _promptSize) % _nbrCol))
    {
        capStr("up");
        while (_nbrCol)
        {
            capStr("nd");
            --_nbrCol;
        }
    }
    else
    {
        capStr("le");
    }
}

/* Move cursor to the left */
int gotoLeft(t_list_cmd ** _cmd, int _key)
{
    int nbrCol;
    int promptSize;
    int i = 0;

    if (_cmd != NULL)
    {
        i = (*_cmd)->index;
    }
    promptSize = getPrompt(NOWRT_PRT);
    _key = 0;
    nbrCol = tgetnum("co");
    if (i)
    {
        findEndLine(i, promptSize, nbrCol);
        i--;
    }
    if (_cmd != NULL)
    {
        (*_cmd)->index = i;
        return (_key);
    }
    return (i);
}

/* Move cursor to the beginning of a line */
int begLine(t_list_cmd ** _cmd, int _key)
{
    _key = 0;
/* While the index is not zero (meaning it's the beginning of th line) */
    while ((*_cmd)->index)
    {
        gotoLeft(_cmd, _key);
    }
    return (_key);
}

/* Move cursor to the end of a line */
int endLine(t_list_cmd ** _cmd, int _key)
{
    _key = 0;
/* While the index is different of the size of the line */
    while ((*_cmd)->line - ((*_cmd)->index))
    {
        gotoRight(_cmd, _key);
    }
    return (_key);
}
