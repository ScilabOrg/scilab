/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __INITSCILABENGINE_HXX__
#define __INITSCILABENGINE_HXX__

extern "C"
{
#include "../../../console/includes/InitializeConsole.h"
#include "../../../jvm/includes/InitializeJVM.h"
#include "InitializeCore.h"
#include "../../../shell/includes/InitializeShell.h"
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
#include "SetScilabEnvironment.h"
#include "../../../jvm/includes/loadBackGroundClassPath.h"

#include "ConsoleRead.h"
/*
** HACK HACK HACK
*/
    extern char *TermReadAndProcess(void);
    extern void ConsolePrintf(char*);
}

#include "yaspio.hxx"

/*
** This function will init scilab engine loading different parts
** like JVM / GUI / CORE / SHELL
**
** There are 2 implementations :
** - initscilabengine-jvm.cpp for scilab && scilab-adv-cli
** - initscilabengine-nojvm.cpp for scilab-cli
*/
void InitScilabEngine(bool consoleMode, bool noJvm);

/*
**
*/
void BindGuiConsoleIO(void);

/*
**
*/
void BindTerminalConsoleIO(void);


#endif /* !__INITSCILABENGINE_HXX__ */
