/*
 ** aff_prompt.c for Scilab Project in /home/mamode/share_blouno/zzledt/tc_prompt
 ** 
 ** Made by MAMODE Karim
 ** Login   <karim.mamode@scilab.org>
 ** 
 ** Started on  Fri May 20 15:51:17 2011 karim mamode
 ** Last update Sun May 22 16:08:40 2011 karim mamode
 */

#include	<wchar.h>
#include	<wctype.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<wchar.h>
#include	<wctype.h>
#include	<curses.h>

#include	"aff_prompt.h"

/*
 * If there's a string, the function save it.
 * The function return the current saved prompt.
 */

wchar_t		*setPrompt(wchar_t *wcs)
{
  static wchar_t	*prompt = NULL;

  if (wcs != NULL)
    prompt = wcs;
  return (prompt);
}

/*
 * The function get the current prompt
 * If none are set, function set it to the basic prompt ("-->")
 * If Argument pass is WRT_PRT (1), it write prompt (NOWRT_PRT (-1) not to write prompt)
 * Function return size of the prompt.
 */

int		getPrompt(int token)
{
  wchar_t		*prompt;

  prompt = setPrompt(NULL);
  if (prompt == NULL)
    prompt = setPrompt(L"-->");
  if (token == WRT_PRT)
    {
      printf("%ls", prompt);
      fflush(stdout);
    }
  return (wcslen(prompt));
}
