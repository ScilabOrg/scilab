/*
** cmd_func.h for reader 42sh in /home/mamode_k//projet_groupe/42sh
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Fri May 20 17:27:36 2011 karim mamode
** Last update Sun May 22 16:32:46 2011 karim mamode
*/

#ifndef		CMD_FUNC_H_
# define	CMD_FUNC_H_

#include		"reader.h"

t_list_cmd		*get_new_cmd(t_list_cmd *last_cmd);
void			get_cmd(t_list_cmd **list_cmd, int *key);
t_list_cmd		*free_cmd(t_list_cmd **cmd);
void			suppress_history(t_list_cmd *cmd, int limit);
void			mem_cmd(t_list_cmd *);

#endif		/* !CMD_FUNC_H_ */
