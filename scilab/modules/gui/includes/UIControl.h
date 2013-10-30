/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __UICONTROL_H__
#define __UICONTROL_H__
/*--------------------------------------------------------------------------*/

#include "dynlib_gui.h"
GUI_IMPEXP int setUICProperties(int objUID, int stackPos, int propertiesCount, char * fname, void * pvApiCtx);

GUI_IMPEXP void getUICProperty(int objUID, char * pname, int stackPos);

GUI_IMPEXP void getUserData(int objUID, int stackPos, void * pvApiCtx);

GUI_IMPEXP void setUserData(int objUID, int stackPos, void * pvApiCtx);

/*--------------------------------------------------------------------------*/
#endif /* __UICONTROL_H__ */
/*--------------------------------------------------------------------------*/
