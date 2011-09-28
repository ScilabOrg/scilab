/*
** reader.h for reader 42sh in /home/mamode_k//projet_groupe/42sh/src/reader
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Sat Apr 16 10:21:24 2011 karim mamode
** Last update Sat May 21 19:35:31 2011 karim mamode
*/

#ifndef		READER_H_
# define	READER_H_

#include	<wchar.h>

typedef struct s_list_cmd t_list_cmd;

struct		s_list_cmd
{
  int			index;
  int			line;
  int			nbr_line;
  int			bin;
  wchar_t		*cmd;
  struct s_list_cmd	*next;
  struct s_list_cmd	*previous;
};

typedef struct	s_func_key
{
  int		key1;
  int		key2;
  int		(*ptr_func)(t_list_cmd **cmd, int key);
}		t_func_key;

int	putstr(wchar_t *);
char	*reader(t_list_cmd **);

t_list_cmd	*core_reader(t_list_cmd *list_cmd);

#endif	/* !READER_H_ */
