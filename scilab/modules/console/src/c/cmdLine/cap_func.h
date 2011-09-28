/*
** cap_str.h for 42sh in /home/mamode_k//projet_groupe/42sh/src/reader
** 
** Made by karim mamode
** Login   <mamode_k@epitech.net>
** 
** Started on  Thu Apr 21 11:06:03 2011 karim mamode
** Last update Fri May 20 17:51:57 2011 karim mamode
*/

#ifndef		CAP_FUNC_H_
# define	CAP_FUNC_H_

#define	SHRD_UP		0x00415b1b
#define	SHRD_DOWN	0x00425b1b
#define	SHRD_RIGHT	0x00435b1b
#define	SHRD_LEFT	0x00445b1b
#define	SHRD_BACKSPACE	0x0000007f
#define	SHRD_PAGE_UP	0x7e355b1b
#define	SHRD_PAGE_DOWN	0x7e365b1b
#define	SHRD_DELETE	0x7e335b1b
#define	SHRD_HOME	0x00484f1b
#define	SHRD_END	0x00464f1b
#define	SHRD_TAB	0x00000009

#define	CTRL_A		0x00000001
#define	CTRL_B		0x00000002
#define	CTRL_D		0x00000004
#define	CTRL_E		0x00000005
#define	CTRL_F		0x00000006
#define	CTRL_I		0x00000009
#define	CTRL_K		0x0000000B
#define	CTRL_M		0x0000000D
#define	CTRL_N		0x0000000E
#define	CTRL_P		0x00000010

#define	META_B		0x0000621b
#define	META_F		0x0000661b

#define	ESCAPE		0x0000001b

void	cap_str(char *str);
void	cap_goto(int col, int li);

#endif /* !CAP_FUNC_H_ */
