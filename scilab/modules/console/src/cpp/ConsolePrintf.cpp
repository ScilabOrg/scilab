/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - Yung-Jang Lee
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "ConsolePrintf.hxx"
/*--------------------------------------------------------------------------*/
#include "CallScilabBridge.hxx"
#include "stack-def.h"
extern "C" {
#include "getScilabJavaVM.h"
#include "stack-def.h"
}

using namespace  org_scilab_modules_gui_bridge;

int ConsolePrintf(char *line)
{
    JavaVM *vm = getScilabJavaVM();
    if (vm == NULL)
    {
        /* Java not yet or badly initialized */
        printf("%s", line);
    }
    else
    {
        CallScilabBridge::display(vm, line);
    }

    return 0;
}
