/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GET_UICONTROL_SELECTED_H__
#define __GET_UICONTROL_SELECTED_H__
#include "dynlib_gui.h"
#include "returnProperty.h"

/**
 * Get the selected property of a uicontrol
 *
 * @param pobjUID Scilab object corresponding to the uicontrol
 * @return true if the font angle property has been correcty got
 */
GUI_IMPEXP int GetUicontrolSelected(void* _pvCtx, int iObjUID);

#endif /* __GET_UICONTROL_SELECTED_H__ */
