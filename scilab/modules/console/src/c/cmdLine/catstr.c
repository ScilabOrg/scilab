/*
 ** catstr.c for reader 42sh in /home/mamode/share_blouno/zzledt/tc_prompt
 ** 
 ** Made by karim mamode
 ** Login   <mamode_k@epitech.net>
 ** 
 ** Started on  Thu May 19 15:56:16 2011 karim mamode
 ** Last update Sun May 22 16:17:13 2011 karim mamode
 */

#include		<unistd.h>
#include		<stdlib.h>
#include		<wchar.h>
#include		<wctype.h>

wchar_t			*catstr(wchar_t *wcs1, wchar_t *wcs2)
{
  wchar_t		*wcs;
  int			i;
  int			n;

  wcs = malloc(sizeof(*wcs) * (1 + wcslen(wcs1) + wcslen(wcs2)));
  if (wcs == NULL)
    exit(EXIT_FAILURE);
  n = 0;
  i = 0;
  while (wcs1[i])
    {
      wcs[n] = wcs1[i];
      i++;
      n++;
    }
  i = 0;
  while (wcs2[i])
    {
      wcs[n] = wcs2[i];
      i++;
      n++;
    }
  wcs[n] = '\0';
  return (wcs);
}
