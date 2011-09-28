/*
 ** cap_func.c for capacitor functions in /home/mamode/share_blouno/zzledt/tc_prompt
 ** 
 ** Made by karim mamode
 ** Login   <mamode_k@epitech.net>
 ** 
 ** Started on  Tue Feb  1 16:21:10 2011 karim mamode
 ** Last update Sat May 21 15:28:26 2011 karim mamode
 */

#include		<term.h>
#include		<curses.h>
#include		<termios.h>
#include		<stdio.h>
#include		<stdlib.h>
#include		<unistd.h>
#include		<stdio.h>

void	cap_str(char *str)
{
  char	*cap_str;

  cap_str = tgetstr(str, NULL);
  if (cap_str != NULL)
    tputs(cap_str, 1, putchar);
}

void	cap_goto(int col, int li)
{
  tputs(tgoto(tgetstr("cm", NULL), col, li), 1, putchar);
}
