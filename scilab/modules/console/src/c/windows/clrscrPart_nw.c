/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include "clrscrPart_nw.h"
#include "TermPosition.h"
/*--------------------------------------------------------------------------*/
void clrscrPart_nw(int nblines)
{
    COORD coord;
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    coord.X = 0;
    coord.Y = (SHORT)(info.dwCursorPosition.Y - nblines - 1) ;

    TermSetPosition(0, coord.Y - 1);

    FillConsoleOutputCharacter (GetStdHandle(STD_OUTPUT_HANDLE), ' ',
                                info.dwSize.X * (info.dwSize.Y - coord.Y - 1), coord, &written);

    TermSetPosition(0, coord.Y - 1);
}
/*--------------------------------------------------------------------------*/
