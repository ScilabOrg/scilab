/*
** goto_func.c for reader 42sh in /home/mamode_k//projet_groupe/42sh/src/reader
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Wed Apr 27 10:35:11 2011 karim mamode
** Last update Sun May 22 18:35:10 2011 karim mamode
*/

#include	<termios.h>
#include	<curses.h>
#include	<term.h>
#include	<string.h>
#include	"reader.h"
#include	"cap_func.h"
#include	"aff_prompt.h"

int		goto_right(t_list_cmd **cmd, int key)
{
  int		nbr_col;
  int		prompt_size;

  prompt_size = getPrompt(NOWRT_PRT);
  key = 0;
  nbr_col = tgetnum("co");
  if ((*cmd)->index != (*cmd)->line)
    {
      if (!(((*cmd)->index + prompt_size + 1) % nbr_col))
	cap_str("do");
      else
      cap_str("nd");
      ((*cmd)->index)++;
    }
  else if (!(((*cmd)->index + prompt_size) % nbr_col))
    cap_str("do");
  return (key);
}


/*
 * go to the last column of the current line
 *
 */

void		find_end_line(int i, int prompt_size, int nbr_col)
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

int		goto_left(t_list_cmd **cmd, int key)
{
  int		nbr_col;
  int		prompt_size;
  int		i;

  if (cmd != NULL)
    i = (*cmd)->index;
  prompt_size = getPrompt(NOWRT_PRT);
  key = 0;
  (void)cmd;
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

int		beg_line(t_list_cmd **cmd, int key)
{
  key = 0;
  while ((*cmd)->index)
    goto_left(cmd, key);
  return (key);
}

int		end_line(t_list_cmd **cmd, int key)
{
  key = 0;
  while ((*cmd)->line - ((*cmd)->index))
    goto_right(cmd, key);
  return (key);
}
