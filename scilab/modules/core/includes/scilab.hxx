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

#ifndef __SCILAB_HXX__
#define __SCILAB_HXX__

/*
-*- Windows main -*-
*/
#ifdef _MSC_VER
#ifndef __WSCILEX__
// Standard Console main
#define __SCILAB_MAIN_BEGIN__ int main(int argc, char *argv[]) {
#define __SCILAB_MAIN_END__ }

#else
// Dedicated main Windows API main
#include <windows.h>

#define __SCILAB_MAIN_BEGIN__ int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow) \
    {                                                                   \
    int argc = 0;                                                       \
    LPWSTR *szArglist;                                                  \
                                                                        \
    szArglist = CommandLineToArgvW(GetCommandLineW(), &argc);           \
    char **argv = (char**) malloc(argc * sizeof(char*));                \
    if (szArglist)                                                      \
    {                                                                   \
        for (int i = 0; i < argc; i++)                                  \
        {                                                               \
            argv[i] = wide_string_to_UTF8(szArglist[i]);                \
        }                                                               \
        /* Free memory allocated for CommandLineToArgvW arguments.*/    \
        LocalFree(szArglist);                                           \
    }

#define __SCILAB_MAIN_END__                                             \
    free(argv);                                                         \
}

#endif
#endif

/*
-*- Linux main -*-
// Fallback in default main prototype
*/

/*
-*- Mac Os main -*-
// Fallback in default main prototype
*/


/* Default case where no main prototype has been defined. */
#if !defined(__SCILAB_MAIN_BEGIN__) && !defined(__SCILAB_MAIN_END__)
#define __SCILAB_MAIN_BEGIN__ int main(int argc, char *argv[]) \
    {
#define __SCILAB_MAIN_END__ }
#endif

#endif /* !__SCILAB_HXX__ */
