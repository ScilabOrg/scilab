/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "ClearConsole.hxx"
/*--------------------------------------------------------------------------*/
#include "CallScilabBridge.hxx"
using namespace  org_scilab_modules_gui_bridge;
BOOL ClearConsole(void)
{
    CallScilabBridge::clear(getScilabJavaVM());
    return TRUE;
}
/*--------------------------------------------------------------------------*/
