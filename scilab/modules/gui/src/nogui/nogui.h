/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre Ledru
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __NOGUI_H__
#define __NOGUI_H__

#include "dynlib_gui.h"
#include "BOOL.h"

GUI_IMPEXP BOOL InitializeGUI(void);

GUI_IMPEXP int gw_gui(void);

GUI_IMPEXP BOOL TerminateGUI(void);

/**
* set Main Window visibility
* @param[in] new state
*/
GUI_IMPEXP void setVisibleMainWindow(BOOL newVisibleState);

/**
* isVisibleMainWindow
* @return current state
*/
GUI_IMPEXP BOOL isVisibleMainWindow(void);

GUI_IMPEXP void deiconifyMainWindow(void);

/**
* Iconify Main Window
*/
GUI_IMPEXP void iconifyMainWindow(void);

/**
* Maximize Main Window
*/
GUI_IMPEXP void maximizeMainWindow(void);

/**
* Main Window is in the "normal" state.
*/
GUI_IMPEXP void normalMainWindow(void);

#endif /* __NOGUI_H__ */

