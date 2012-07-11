/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GET_UICONTROL_FONTWEIGHT_H__
#define __GET_UICONTROL_FONTWEIGHT_H__
#include "dynlib_gui.h"
#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the weight of a uicontrol font
 *
 * @param pobjUID Scilab object corresponding to the uicontrol
 * @return true if the font has been correcty got
 */
GUI_IMPEXP int GetUicontrolFontWeight(void* _pvCtx, char *pobjUID);

#endif /* __GET_UICONTROL_FONTWEIGHT_H__ */
