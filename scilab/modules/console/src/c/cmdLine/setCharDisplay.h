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

#ifndef SETCHARDISPLAY_H_
#define SETCHARDISPLAY_H_

#include <stdlib.h>

/* Some Characteritics and colors */

/* Characteristic */
#define DEFAULT "0"
#define BRIGHT "1"
#define FAINT "2"
#define ITALIC "3"
#define NEGATIVE "7"
#define LAST_SET NULL

/* Character Color */
#define CHAR_DEFAULT "0"
#define CHAR_BLACK "30"
#define CHAR_RED "31"
#define CHAR_GREEN "32"
#define CHAR_YELLOW "33"
#define CHAR_BLUE "34"
#define CHAR_MAGENTA "35"
#define CHAR_CYAN "36"
#define CHAR_WHITE "37"

/* Background Color */
#define BKGD_DEFAULT "0"
#define BKGD_BLACK "40"
#define BKGD_RED "41"
#define BKGD_GREEN "42"
#define BKGD_YELLOW "43"
#define BKGD_BLUE "44"
#define BKGD_MAGENTA "45"
#define BKGD_CYAN "46"
#define BKGD_WHITE "47"

/* Reset settings */
#define RESET "0"

/**
 * Set character display characteristic.
 * @param Characteristic to set.
 */
void setCharDisplay(const char *characteristic);

#endif /* SETCHARDISPLAY_H_ */
