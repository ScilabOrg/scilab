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

int		nextWord(t_list_cmd **_cmd, int _key)
{
  while ((*_cmd)->cmd[(*_cmd)->index] && (*_cmd)->cmd[(*_cmd)->index] != L' ')
    {
      goto_right(_cmd, _key);
    }
  while ((*_cmd)->cmd[(*_cmd)->index] && (*_cmd)->cmd[(*_cmd)->index] == L' ') /* then passing through spaces */
    {
      goto_right(_cmd, _key);
    }
  _key = 0;
  return (_key);
}

int		previousWord(t_list_cmd **_cmd, int _key)
{
  while ((*_cmd)->index && (*_cmd)->cmd[(*_cmd)->index - 1] == L' ')
    {
      goto_left(_cmd, _key);
    }
  while ((*_cmd)->index && (*_cmd)->cmd[(*_cmd)->index - 1] != L' ')
    {
      goto_left(_cmd, _key);
    }
  _key = 0;
  return (_key);
}
