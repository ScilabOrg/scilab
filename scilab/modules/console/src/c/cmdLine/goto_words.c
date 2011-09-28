/*
** goto_words.c for reader in /home/mamode//src/reader
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Fri Sep  1 15:39:48 2011 karim mamode
** Last update Fri Sep  1 17:06:54 2011 karim mamode
*/

#include	"reader.h"
#include	"goto_func.h"

int		next_word(t_list_cmd **cmd, int key)
{
  if ((*cmd)->cmd[(*cmd)->index])
    goto_right(cmd, key);
  while ((*cmd)->cmd[(*cmd)->index] && (*cmd)->cmd[(*cmd)->index] != L' ')
    goto_right(cmd, key);
  key = 0;
  return (key);
}

int		previous_word(t_list_cmd **cmd, int key)
{
  if ((*cmd)->index)
    goto_left(cmd, key);
  while ((*cmd)->index && (*cmd)->cmd[(*cmd)->index -1] != L' ')
    goto_left(cmd, key);
  key = 0;
  return (key);
}
