/*
** init_tc_shell.c for reader 42sh in /home/mamode_k//projet_groupe/42sh/src/reader
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Sat Apr 16 11:18:51 2011 karim mamode
** Last update Sat May 21 15:34:02 2011 karim mamode
*/

#include		<termios.h>
#include		<curses.h>
#include		<term.h>
#include		<unistd.h>
#include		<stdlib.h>
#include		<stdio.h>
#include		<locale.h>
#include		"init_tc_shell.h"

int			canonic_mode(struct termios *t)
{
  t->c_lflag |= ICANON;
  t->c_lflag |= ECHO;
  return (0);
}

int			raw_mode(struct termios *t)
{
  t->c_lflag &= ~ICANON;
  t->c_lflag &= ~ECHO;
  t->c_cc[VMIN] = 1;
  t->c_cc[VTIME] = 0;
  return (0);
}

void			set_attr(int bin)
{
  struct termios	t;

  if (tcgetattr(0, &t) == -1)
    {
      puts("Cannot access to the term attributes.");
      exit(EXIT_FAILURE);
    }
  if (bin == CANON)
    canonic_mode(&t);
  else
    raw_mode(&t);
  if (tcsetattr(0, 0, &t) == -1)
    {
      puts("Cannot change the term attributes.");
      exit(EXIT_FAILURE);
    }
}

int			init_shell(int bin)
{
  if (tgetent(NULL, getenv("TERM")) == ERR
      && tgetent(NULL, "xterm") == ERR
      && tgetent(NULL, "linux") == ERR)
    {
      puts("Cannot init termcaps");
      return (2);
    }
  set_attr(bin);

  if (setlocale(LC_CTYPE, "") == NULL)
    {
      puts("Cannot set wide char.");
      return (2);
    }

  return (0);
}
