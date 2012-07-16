/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETUIMENUCHECKED_H__
#define __GETUIMENUCHECKED_H__
#include "dynlib_gui.h"
#include "returnProperty.h"

/**
 * Get the checked status of an uimenu
 *
 * @param pobjUID UID of the uimenu
 * @return true if the status is got without error
 */
GUI_IMPEXP int GetUimenuChecked(void* _pvCtx, char *pobjUID);

#endif /* !__GETUIMENUCHECKED_H__ */
