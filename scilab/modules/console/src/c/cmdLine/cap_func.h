/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#ifndef		CAP_FUNC_H_
#define	CAP_FUNC_H_

/* TODO comment where these values are from.
* Are you sure you cannot find them in an header file provided by terminfo/termcap ?? 
*/
/* TODO comment */
#define	SHRD_UP		0x00415b1b
/* TODO comment */
#define	SHRD_DOWN	0x00425b1b
/* TODO comment */
#define	SHRD_RIGHT	0x00435b1b
/* TODO comment */
#define	SHRD_LEFT	0x00445b1b
/* TODO comment */
#define	SHRD_BACKSPACE	0x0000007f
/* TODO comment */
#define	SHRD_PAGE_UP	0x7e355b1b
/* TODO comment */
#define	SHRD_PAGE_DOWN	0x7e365b1b
/* TODO comment */
#define	SHRD_DELETE	0x7e335b1b
/* TODO comment */
#define	SHRD_HOME	0x00484f1b
/* TODO comment */
#define	SHRD_END	0x00464f1b
/* TODO comment */
#define	SHRD_TAB	0x00000009

/* TODO comment + etc */
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

/* TODO comment */
void cap_str(char *str);

/* TODO comment */
void cap_goto(int col, int li);

#endif /* !CAP_FUNC_H_ */
