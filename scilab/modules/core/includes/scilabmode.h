/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __SCILABMODE_H__
#define __SCILABMODE_H__

#include "machine.h"

typedef enum {
	SCILAB_API=1, /* Scilab is launch as an API */
	SCILAB_STD, /* The standard Scilab (gui, plot ...) */
	SCILAB_NW, /* Scilab in command line with the plots */
	SCILAB_NWNI /* Scilab in command line without any graphics. What not mandataroy here */
} scilabMode;

/* by default mode is API */

/**
* Set scilab mode 
* @param new scilab mode
* @return TRUE is newmode is OK
*/
BOOL setScilabMode(scilabMode newmode);

/**
* Get scilab mode
* @return current scilab mode
*/
scilabMode getScilabMode(void);

/**
* Get scilab mode as a string
* @return current scilab mode
*/
char * getScilabModeString(void);


/**
 * Gives the mode of scilab
 * @param[out] the mode of scilab
 * @return 1
 */
int C2F(getscilabmode)(scilabMode *mode);


#endif /* __SCILABMODE_H__ */
/*--------------------------------------------------------------------------*/ 
