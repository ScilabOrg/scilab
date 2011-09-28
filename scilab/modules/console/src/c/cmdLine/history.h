/*
** history.h for reader 42sh in /home/mamode_k//projet_groupe/42sh/src/reader
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Thu Apr 28 11:05:09 2011 karim mamode
** Last update Fri May 20 10:35:32 2011 karim mamode
*/

#ifndef		HISTORY_H_
# define	HISTORY_H_

#include	"reader.h"

int		previous_cmd(t_list_cmd **cmd, int key);
int		next_cmd(t_list_cmd **cmd, int key);

#endif	/* !HISTORY_H_ */
