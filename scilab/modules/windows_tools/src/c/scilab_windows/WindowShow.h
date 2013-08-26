/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __WINDOWSHOW_H__
#define __WINDOWSHOW_H__

#include "dynlib_scilab_windows.h"

/**
* set Current Window Mode Show
* @param[IN] set Current Window Mode Show
*/
SCILAB_WINDOWS_IMPEXP void setWindowShowMode(int nCmdShow);

/**
* get Current Window Show Mode
* @return current Window Show Mode
*/
SCILAB_WINDOWS_IMPEXP int getWindowShowMode(void);

/**
* update Window show with current Mode
*/
SCILAB_WINDOWS_IMPEXP void WindowShow(void);

#endif /* __WINDOWSHOW_H__*/
/*--------------------------------------------------------------------------*/
