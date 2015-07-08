/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU (Detection of the term size)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "machine.h"

/* for getenv() */
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifndef _MSC_VER

#  if defined(HAVE_CURSES_H)
#    include <curses.h>
#  elif defined(HAVE_NCURSES_H)
#    include <ncurses.h>
#  endif


#ifdef HAVE_TERMCAP_H
#include <termcap.h>
#endif

#ifndef HAVE_TERMCAP_H
#ifdef HAVE_TERM_H
#include <term.h>
#endif
#endif

#endif /* !defined(_MSC_VER) */

#include "scilines.h"
#include "core_math.h"
#ifdef _MSC_VER
#include "../../../windows_tools/src/c/scilab_windows/console.h"
#endif
#include "configvariable_interface.h"
/*--------------------------------------------------------------------------*/
#define DEFAULT_NUMBERS_LINES 28
#define DEFAULT_NUMBERS_COLUMNS 80
#define MIN_NUMBERS_LINES 0
#define MIN_NUMBERS_COLUMNS 10
/*--------------------------------------------------------------------------*/
int scilines(int nblines, int nbcolumns)
{
    setConsoleLines(nblines);
    setConsoleWidth(nbcolumns);
    return 0;
}
/*--------------------------------------------------------------------------*/
int scilinesdefault(void)
{
#ifndef _MSC_VER

    char tc_buf[1024];       /* holds termcap buffer */
    if (tgetent(tc_buf, getenv("TERM")) == 1)
    {
        setConsoleLines(tgetnum("li")); /* retrieve from the term info the number
										of lines */
        setConsoleWidth(tgetnum("co")); /* And the number of columns */
    }
    else
    {
        /* Haven't been able to detect the terminal */
        setConsoleLines(DEFAULT_NUMBERS_LINES);
        setConsoleWidth(DEFAULT_NUMBERS_COLUMNS);
    }

#else
    if ( (getScilabMode() != SCILAB_STD) && (getScilabMode() != SCILAB_API) )
    {
        /* -NW or -NWNI mode */
        int X = getXConsoleScreenSize();
        int Y = getYConsoleScreenSize();

        if (X < DEFAULT_NUMBERS_COLUMNS)
        {
            X = DEFAULT_NUMBERS_COLUMNS;
        }
        if (Y < DEFAULT_NUMBERS_LINES)
        {
            Y = DEFAULT_NUMBERS_LINES;
        }
        setConsoleWidth(X);
        setConsoleLines(Y);
    }
    else
    {
        setConsoleLines(DEFAULT_NUMBERS_LINES);
        setConsoleWidth(DEFAULT_NUMBERS_COLUMNS);
    }
#endif
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(scilines)(int *nblines, int *nbcolumns)
{
    return scilines(*nblines, *nbcolumns);
}
/*--------------------------------------------------------------------------*/
int C2F(scilinesdefault)(void)
{
    return scilinesdefault();
}
/*--------------------------------------------------------------------------*/
