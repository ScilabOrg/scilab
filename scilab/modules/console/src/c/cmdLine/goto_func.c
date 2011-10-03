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

/* TODO merge goto_words into goto_funcs */
#include	<termios.h>
#include	<curses.h>
#include	<term.h>
#include	<string.h>
#include	"reader.h"
#include	"cap_func.h"
#include	"aff_prompt.h"

int goto_right(t_list_cmd ** cmd, int key)
{
    int nbr_col;
    int prompt_size;

    prompt_size = getPrompt(NOWRT_PRT);
    key = 0;
    nbr_col = tgetnum("co");
    if ((*cmd)->index != (*cmd)->line)
    {
/* TODO comment */
        if (!(((*cmd)->index + prompt_size + 1) % nbr_col))
            cap_str("do");
        else
            cap_str("nd");
        ((*cmd)->index)++;
    }
/* TODO comment */
    else if (!(((*cmd)->index + prompt_size) % nbr_col))
        cap_str("do");
    return (key);
}

/*
 * go to the last column of the current line
 *
 */

void find_end_line(int i, int prompt_size, int nbr_col)
{
    if (!((i + prompt_size) % nbr_col))
    {
        cap_str("up");
        while (nbr_col)
        {
            cap_str("nd");
            --nbr_col;
        }
    }
    else
        cap_str("le");
}

int goto_left(t_list_cmd ** cmd, int key)
{
    int nbr_col;
    int prompt_size;
    int i;

/* TODO comment the function */
    if (cmd != NULL)
        i = (*cmd)->index;
    prompt_size = getPrompt(NOWRT_PRT);
    key = 0;
    (void)cmd;                  /* What is it supposed to do ? */
    nbr_col = tgetnum("co");
    if (i)
    {
        find_end_line(i, prompt_size, nbr_col);
        i--;
    }
    if (cmd != NULL)
    {
        (*cmd)->index = i;
        return (key);
    }
    return (i);
}

int beg_line(t_list_cmd ** cmd, int key)
{
    key = 0;
/* TODO comment  */
    while ((*cmd)->index)
        goto_left(cmd, key);
    return (key);
}

int end_line(t_list_cmd ** cmd, int key)
{
    key = 0;
/* TODO comment  */
    while ((*cmd)->line - ((*cmd)->index))
        goto_right(cmd, key);
    return (key);
}
