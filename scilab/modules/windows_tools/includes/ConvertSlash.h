/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __CONVERTSLASH_H__
#define __CONVERTSLASH_H__

#include "BOOL.h"


/* C:\Windows\system and C:/Windows/system */
/* since Windows 2000 SP2 are supported */

/**
* convert a unix path to windows path 'format'
* replaces '/' by '\'
* @param [in] path in with '/'
* @param [out] path out with '\'
* @return TRUE or FALSE
*/
BOOL slashToAntislash(char *pathunix, char *pathwindows);

/**
* convert a windows path to unix path 'format'
* replaces '\' by '/'
* @param [in] path in with '\'
* @param [out] path out with '/'
* @return TRUE or FALSE
*/
BOOL AntislashToSlash(char *pathwindows, char *pathunix);

#endif /* __CONVERTSLASH_H__ */
/*--------------------------------------------------------------------------*/
