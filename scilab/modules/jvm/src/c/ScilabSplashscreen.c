/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <stdio.h>

#ifdef _MSC_VER
static const char* SPLASHSCREEN_SO = "\\bin\\splashscreen.dll";
static HMODULE hSplashLib = NULL;
#else
static const char* SPLASHSCREEN_SO = "libsplashscreen.so";
static void* hSplashLib = NULL;
#endif

static void* SplashProcAddress(const char* name);

/**
 * Show a splash screen until the first java window appears
 * @param the image file name
 */
void ShowSplashScreen(const char* name)
{
        if (name)
        {
                void (*SplashInit)(void) = NULL;
                int (*SplashLoadFile)(const char*) = NULL;

                SplashInit = SplashProcAddress("SplashInit");
                SplashLoadFile = SplashProcAddress("SplashLoadFile");

                if (SplashInit && SplashLoadFile)
                {
                        SplashInit();
                        SplashLoadFile(name);
                }
        }
}

/**
 * Free the library used to show th splash screen.
 */
void SplashFreeLibrary()
{
        if (hSplashLib)
        {
#ifdef _MSC_VER
                FreeLibrary(hSplashLib);
#else
                dlclose(hSplashLib);
#endif
                hSplashLib = NULL;
        }
}

static void* SplashProcAddress(const char* name)
{
        if (!hSplashLib)
        {
#ifdef _MSC_VER
                /* Build the library path */
                //libraryPath = jrePath + SPLASHSCREEN_SO
                //hSplashLib = LoadLibrary(libraryPath);
#else
                hSplashLib = dlopen(SPLASHSCREEN_SO, RTLD_LAZY | RTLD_GLOBAL);
#endif
        }

        if (hSplashLib)
        {
#ifdef _MSC_VER
                return GetProcAddress(hSplashLib, name);
#else
                return dlsym(hSplashLib, name);
#endif
        }
        else
        {
                return NULL;
        }
}
