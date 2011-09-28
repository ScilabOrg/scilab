/*
** history.c for reader 42sh in /home/mamode_k//projet_groupe/42sh/src/reader
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Thu Apr 28 11:01:35 2011 karim mamode
** Last update Sat May 21 17:56:25 2011 karim mamode
*/

#include	<stdio.h>
#include	<unistd.h>
#include	<term.h>
#include	<termios.h>
#include	"cap_func.h"
#include	"goto_func.h"
#include	"aff_prompt.h"
#include	"reader.h"

int		previous_cmd(t_list_cmd **cmd, int key)
{
  int		prompt_size;

  key = 0;
  if ((*cmd)->previous)
    {
      while ((*cmd)->index)
	goto_left(cmd, key);
      cap_str("up");
      cap_str("do");
      cap_str("cd");
      prompt_size = getPrompt(WRT_PRT);
      (*cmd) = (*cmd)->previous;
      printf("%ls", (*cmd)->cmd);
      fflush(stdout);
      (*cmd)->index = (*cmd)->line;
      if (!(((*cmd)->index + prompt_size) % tgetnum("co")))
      	cap_str("do");
    }
  return (key);
}

int		next_cmd(t_list_cmd **cmd, int key)
{
  int		prompt_size;

  key = 0;
  if ((*cmd)->next)
    {
      while ((*cmd)->index)
	goto_left(cmd, key);
      cap_str("up");
      cap_str("do");
      cap_str("cd");
      (*cmd) = (*cmd)->next;
      prompt_size = getPrompt(WRT_PRT);
      printf("%ls", (*cmd)->cmd);
      fflush(stdout);
      (*cmd)->index = (*cmd)->line;
      if (!(((*cmd)->index + prompt_size) % tgetnum("co")))
	cap_str("do");
    }
  return (key);
}
