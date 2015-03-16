/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "stdio.h"


#include "InitializeTclTk.h"
#include "TerminateTclTk.h"

#ifdef _MSC_VER
#include <windows.h>
int WINAPI DllMain(HINSTANCE hInstance, DWORD reason, PVOID pvReserved);
#endif


#ifdef _MSC_VER
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"../../../../bin/libintl.lib")
/*--------------------------------------------------------------------------*/
int WINAPI DllMain(HINSTANCE hInstance, DWORD reason, PVOID pvReserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            InitializeTclTk();
            break;
        case DLL_PROCESS_DETACH:
            dynamic_TerminateTclTk();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return 1;
}
#else
__attribute__ ((constructor)) static void load(void);
__attribute__ ((destructor)) static void unload(void);

void load(void)
{
    static initialized = 0;
    if (initialized == 0)
    {
        InitializeTclTk();
        initialized = 1;
    }
}

void unload(void)
{
    TerminateTclTk();
}
#endif

/*--------------------------------------------------------------------------*/

