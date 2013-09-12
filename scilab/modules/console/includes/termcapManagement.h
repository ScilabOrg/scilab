/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#ifndef CAP_FUNC_H_
#define CAP_FUNC_H_

/* TODO: Check if it is really portable */

/* Token used to differenciate DEL key from Backspace key for rmChar */

/* When backspace is pressed */
#define SCI_BACKSPACE 0x7f
/* when delete is pressed */
#define SCI_DELETE 0X1b5b337e

/* Value returned by getchar/getwchar by pressing ctrl + key */
#define CTRL_A 0x01
#define CTRL_B 0x02
#define CTRL_D 0x04
#define CTRL_E 0x05
#define CTRL_F 0x06
#define CTRL_H 0x08
#define CTRL_I 0x09
#define CTRL_K 0x0B
#define CTRL_L 0x0C
#define CTRL_M 0x0D
#define CTRL_N 0x0E
#define CTRL_P 0x10
#define CTRL_U 0x15
#define CTRL_W 0x17

/* Define for meta key (alt + key or escape) */
#define ESCAPE 0x1b

/**
 * Turn a capacity on, Useful to simplify termcap use.
 * TODO: please rename for something more explicit (startTermcap ?)
 * @param capacity to activate.
 */
void setStringCapacities(const char *str);

/**
 * Move cursor using termcap, Useful to simplify termcap use.
 * Note: Can be consider as x,y coordinates.
 * @param column where cursor will move
 * @param line where cursor will move
 */
void capGoto(int col, int li);

#endif /* !CAP_FUNC_H_ */
