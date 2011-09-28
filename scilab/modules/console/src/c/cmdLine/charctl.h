/*
** charctl.h for reader 42sh in /home/mamode_k//projet_groupe/42sh/src/reader
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Wed Apr 27 10:31:17 2011 karim mamode
** Last update Fri May 20 10:19:58 2011 karim mamode
*/

#ifndef		CHAR_CTL_H_
# define	CHAR_CTL_H_

#include	"reader.h"

int		add_letter(t_list_cmd **cmd, int key);
int		rm_letter(t_list_cmd **cmd, int key);
int		delete_line_from_curs(t_list_cmd **cmd, int key);

#endif	/* !CHAR_CTL_H_*/
