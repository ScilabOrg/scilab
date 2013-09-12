/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "InitScilab.h"
#include "InitializeCore.h"
#include "../../../console/includes/InitializeConsole.h"
#include "../../../tclsci/includes/InitializeTclTk.h"
#include "../../../localization/includes/InitializeLocalization.h"
#include "../../../graphics/includes/graphicModuleLoad.h"
#include "../../../jvm/includes/InitializeJVM.h"
#ifdef _MSC_VER
#include "../../../windows_tools/includes/InitializeWindows_tools.h"
#endif
#include "../../../gui/includes/InitializeGUI.h"
#include "../../../string/includes/InitializeString.h"
#include "scilabmode.h"
#include "SetScilabEnvironment.h"
#include "../../../jvm/includes/loadBackGroundClassPath.h"
/*--------------------------------------------------------------------------*/
int C2F(initscilab) (void)
{

    SetScilabEnvironment();

    InitializeString();

    InitializeLocalization();

#ifdef _MSC_VER
    InitializeWindows_tools();
#endif

    InitializeCore();

    if (getScilabMode() != SCILAB_NWNI)
    {
        InitializeJVM();
        InitializeGUI();

        /* create needed data structure if not already created */
        loadGraphicModule();
    }

    /* Initialize console: lines... */
    InitializeConsole();

    if (getScilabMode() != SCILAB_NWNI)
    {
        loadBackGroundClassPath();
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
