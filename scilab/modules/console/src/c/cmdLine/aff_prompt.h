/*
** aff_prompt.h for reader in /home/mamode_k//reader/include
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Fri May 20 16:45:25 2011 karim mamode
** Last update Fri May 20 16:49:10 2011 karim mamode
*/

#ifndef		AFF_PROMPT_H_
# define	AFF_PROMPT_H_

# define WRT_PRT	1
# define NOWRT_PRT	-1

#include    <wchar.h>

wchar_t	*setPrompt(wchar_t *);
int	getPrompt(int);

#endif		/* !AFF_PROMPT_H_ */
