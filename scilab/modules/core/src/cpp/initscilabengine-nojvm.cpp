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

#include <iostream>
#include "initscilabengine.hxx"

static void TermPrintf(char *text)
{
    std::cout << text;
}

void InitScilabEngine(bool consoleMode, bool noJvm)
{
    InitializeString();

#ifdef _MSC_VER
    InitializeWindows_tools();
#endif

    InitializeCore();

    InitializeShell();

    if (!noJvm) 
    {
        /* bug 3702 */
        /* tclsci creates a TK window on Windows */
        /* it changes focus on previous windows */
        /* we put InitializeTclTk before InitializeGUI */
        
        //InitializeTclTk();
        InitializeJVM();
        InitializeGUI();

        /* create needed data structure if not already created */
        loadGraphicModule() ;

        /* Standard mode -> init Java Console */
        if ( !consoleMode ) 
        {
            /* Initialize console: lines... */
            InitializeConsole();
        }

        loadBackGroundClassPath();
    }
}


/*
** When compiling scilab-cli, there is no GUI console.
*/
void BindGuiConsoleIO(void)
{
    BindTerminalConsoleIO();
}

void BindTerminalConsoleIO(void)
{
    setYaspInputMethod(&TermReadAndProcess);
    setYaspOutputMethod(&TermPrintf);
}
