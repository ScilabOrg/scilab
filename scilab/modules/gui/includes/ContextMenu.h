/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
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

#ifndef __CONTEXTMENU_H__
#define __CONTEXTMENU_H__

#include "returnProperty.h"

/**
 * Display the ContextMenu and waits for a user input
 *
 * @param sciObj Scilab corresponding object
 */
char *uiWaitContextMenu(int iObjUID);

#endif /* __CONTEXTMENU_H__ */
