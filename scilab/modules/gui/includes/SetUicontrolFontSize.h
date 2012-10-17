/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SET_UICONTROL_FONTSIZE_H__
#define __SET_UICONTROL_FONTSIZE_H__

#include <stddef.h>

#include "dynlib_gui.h"

/**
 * Set the font size of a uicontrol
 *
 * @param pobjUID Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the font size to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the size property set has been correcty done
 */
GUI_IMPEXP int SetUicontrolFontSize(void* _pvCtx, char* pobjUID, void* _pvData, int valueType, int nbRow, int nbCol);

#endif /* __SET_UICONTROL_FONTSIZE_H__ */
