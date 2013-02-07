/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CallScilabBridge.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "RaiseWindow.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;

/*--------------------------------------------------------------------------*/
void raiseWindow(char * id)
{
    CallScilabBridge::raiseWindow(getScilabJavaVM(), id);
}

/*--------------------------------------------------------------------------*/
