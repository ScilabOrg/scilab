/*
** string_func.c for reader 42sh in /home/goya_p//prog/projets_groupe/42sh
** 
** Made by pierre-alexandre goya
** Login   <goya_p@epitech.net>
** 
** Started on  Thu May  5 18:41:03 2011 pierre-alexandre goya
** Last update Sat May 21 13:05:02 2011 karim mamode
*/

#include	<unistd.h>
#include	<string.h>

int		putstr(char *str)
{
  return (write(1, str, strlen(str)));
}
