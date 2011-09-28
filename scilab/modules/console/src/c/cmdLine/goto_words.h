 /*
 ** goto_words.h for reader in /home/mamode//include/reader
 ** 
 ** Made by karim mamode
 ** Login   <mamode_k@epitech.net>
 ** 
 ** Started on  Fri Sep  1 15:39:48 2011 karim mamode
 ** Last update Fri Sep  1 17:06:54 2011 karim mamode
 */

#ifndef		GOTO_WORDS_H_
# define	GOTO_WORDS_H_

#include	"reader.h"

int		next_word(t_list_cmd **cmd, int key);
int		previous_word(t_list_cmd **cmd, int key);

#endif /* !GOTO_WORDS_H_ */
