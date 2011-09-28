/*
** goto_func.h for reader 42sh in /home/mamode_k//projet_groupe/42sh/src/reader
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Wed Apr 27 10:36:34 2011 karim mamode
** Last update Fri May 20 10:20:37 2011 karim mamode
*/

#ifndef		GOTO_FUNC_H_
# define	GOTO_FUNC_H_

#include	"reader.h"

int		goto_right(t_list_cmd **cmd, int key);
int		goto_left(t_list_cmd **cmd, int key);
int		end_line(t_list_cmd **cmd, int key);
int		beg_line(t_list_cmd **cmd, int key);

#endif /* !GOTO_FUNC_H_ */
