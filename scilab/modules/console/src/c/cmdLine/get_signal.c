/*
 ** signal.c for reader in /home/mamode/share_blouno/zzledt/tc_prompt
 ** 
 ** Made by karim mamode
 ** Login   <mamode_k@epitech.net>
 ** 
 ** Started on  Sat Apr 16 10:16:46 2011 karim mamode
 ** Last update Sun May 22 21:25:02 2011 karim mamode
 */

#include	<unistd.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<curses.h>
#include	<term.h>
#include	"cmd_func.h"
#include	"cap_func.h"
#include	"aff_prompt.h"

void		get_new_term(int signum)
{
  char		*term_env;

  cap_str("cl");
  if ((term_env = getenv("TERM")) == NULL)
    exit(EXIT_FAILURE);
  if (tgetent(NULL, term_env) == ERR)
    exit(EXIT_FAILURE);
  getPrompt(WRT_PRT);
  mem_cmd(NULL);
  signal(signum, get_new_term);
}
