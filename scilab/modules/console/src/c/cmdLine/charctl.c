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

#include	<curses.h>
#include	<stdlib.h>
#include	<term.h>
#include	<termios.h>
#include	<unistd.h>
#include	<wchar.h>
#include	<wctype.h>
#include	"reader.h"
#include	"cap_func.h"
#include	"goto_func.h"
#include	"aff_prompt.h"
#include	"charctl.h"

/* Add a character to a command line */
int addLetter(t_list_cmd ** _cmd, int _key)
{
    int indexToMoveChar = 0;
    int promptSize;
    int saveLine;

    promptSize = getPrompt(NOWRT_PRT);
    if (' ' <= _key)
    {
        if ((*_cmd)->line && !((*_cmd)->line % 1024))
        {
            (*_cmd)->cmd = realloc((*_cmd)->cmd, (*(*_cmd)->cmd) * (*_cmd)->line + 1024);
        }
        indexToMoveChar = (*_cmd)->line;
        /* move each character to the next place */
        while (indexToMoveChar > (*_cmd)->index)
        {
            (*_cmd)->cmd[indexToMoveChar] = (*_cmd)->cmd[indexToMoveChar - 1];
            indexToMoveChar--;
        }
        /* Add the new character to the command line. */
        (*_cmd)->cmd[(*_cmd)->index] = (wchar_t) _key;
        printf("%lc", (*_cmd)->cmd[(*_cmd)->index]);
        saveLine = (*_cmd)->nbr_line;
        (*_cmd)->line++;
        /*
         * Nummber of line is equal to :
         * the size of the command line plus the size of the prompt
         * divided by the number of column in the window
         * Plus one.
         */
        (*_cmd)->nbr_line = 1 + ((*_cmd)->line + promptSize + 1) / tgetnum("co");
        (*_cmd)->cmd[(*_cmd)->line] = L'\0';
        (*_cmd)->index++;
        indexToMoveChar++;
        capStr("sc");
        printf("%ls", &(*_cmd)->cmd[indexToMoveChar]);
        fflush(stdout);
        capStr("rc");
        if (!(((*_cmd)->index + promptSize) % tgetnum("co")))
        {
            capStr("do");
        }
    }
    return (_key);
}

/* Delete a character in the command line */
int rmLetter(t_list_cmd ** _cmd, int _key)
{
    int indexToMoveChar;
    int promptSize;

    promptSize = getPrompt(NOWRT_PRT);
/*
 * Case Backspace is pressed -> cursor must not be at the beginning of the command line
 * Case Delete is pressed -> cursor must not be at the end of line
 */
    if (((*_cmd)->index && _key == SCI_BACKSPACE) || (((*_cmd)->line != (*_cmd)->index) && _key == SCI_DELETE))
    {
        if (_key == SCI_BACKSPACE)
        {
            gotoLeft(_cmd, _key);
        }
        indexToMoveChar = (*_cmd)->index;
        capStr("sc");
        while (indexToMoveChar < (*_cmd)->line)
        {
            /* move each character to the previous place and print it */
            (*_cmd)->cmd[indexToMoveChar] = (*_cmd)->cmd[indexToMoveChar + 1];
            printf("%lc", (*_cmd)->cmd[indexToMoveChar]);
            indexToMoveChar++;
        }
        fflush(stdout);
        (*_cmd)->cmd[indexToMoveChar] = L'\0';
        (*_cmd)->line--;
        /*
         * Nummber of line is equal to :
         * the size of the command line plus the size of the prompt
         * divided by the number of column in the window
         * Plus one.
         */
        (*_cmd)->nbr_line = 1 + ((*_cmd)->line + promptSize) / tgetnum("co");
        putchar(' ');
        capStr("rc");
    }
    _key = 0;
    return (_key);
}

/* Delete all characters from cursor to the end. */
int deleteLineFromCurs(t_list_cmd ** _cmd, int _key)
{
    /* The character at the cursor is '\0' mean this is the last */
    while ((*_cmd)->cmd[(*_cmd)->index])
        rmLetter(_cmd, SCI_DELETE);
    _key = 0;
    return (_key);
}
