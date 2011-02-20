/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __SCILABSPLASHSCREEN_H__
#define __SCILABSPLASHSCREEN_H__

/**
* Show a splash screen before the first window is loaded
* @param name the image file name
*/
void ShowSplashScreen(const char *name);

/**
 * Free the splashscreen library
 */
void SplashFreeLibrary();

#endif /* __SCILABSPLASHSCREEN_H__ */
/*--------------------------------------------------------------------------*/
