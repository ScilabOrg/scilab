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

#include		<term.h>
#include		<curses.h>
#include		<termios.h>
#include		<stdio.h>
#include		<stdlib.h>
#include		<unistd.h>
#include		<stdio.h>

/* Simplify termcap activation */
void capStr(char *_capacity)
{
    char *capStr;

    capStr = tgetstr(_capacity, NULL);
    if (capStr != NULL)
    {
        tputs(capStr, 1, putchar);
    }
}

/* Move cursor to the column _col and the line _li */
void capGoto(int _col, int _li)
{
    char *capStr;

    capStr = tgetstr("cm", NULL);
    if (capStr != NULL)
    {
        capStr = tgoto(capStr, _col, _li);
    }
    if (capStr != NULL)
    {
        capStr = tputs(capStr, 1, putchar);
    }
}
