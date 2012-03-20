/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <windows.h>
#ifdef USE_F2C
#include "f2c.h"
#endif
/*-----------------------------------------------------------------------------------*/
#pragma comment(lib, "../../bin/libxml2.lib")
#pragma comment(lib, "../../bin/libintl.lib")
/*-----------------------------------------------------------------------------------*/
/* force external linking fortran libraries */
#pragma comment(lib, "../../bin/blasplus.lib")
#pragma comment(lib, "../../bin/lapack.lib")
/*-----------------------------------------------------------------------------------*/
/* linking fortran libraries of Scilab */
#pragma comment(lib, "../../libs/core_f.lib") /* static others are dynamic*/
#pragma comment(lib, "../../bin/boolean_f.lib")
#pragma comment(lib, "../../bin/data_structures_f.lib")
#pragma comment(lib, "../../bin/elementary_functions_f.lib")
#pragma comment(lib, "../../bin/integer_f.lib")
#pragma comment(lib, "../../bin/io_f.lib")
#pragma comment(lib, "../../bin/linpack_f.lib")
#pragma comment(lib, "../../bin/output_stream_f.lib")
#pragma comment(lib, "../../bin/polynomials_f.lib")
#pragma comment(lib, "../../bin/sparse_f.lib")
#pragma comment(lib, "../../bin/string_f.lib")
/*-----------------------------------------------------------------------------------*/
int WINAPI DllMain (HINSTANCE hInstance , DWORD reason, PVOID pvReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return 1;
}
/*-----------------------------------------------------------------------------------*/
BOOL BuildWithVS8ExpressF2C(void)
{
#ifdef USE_F2C
    return TRUE;
#else
    return FALSE;
#endif
}
/*-----------------------------------------------------------------------------------*/
