/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GETWINDOWSVERSION_H__
#define __GETWINDOWSVERSION_H__
#include <windows.h>
/*-----------------------------------------------------------------------------------*/
#ifdef EXPORT_GETWINDOWSVERSION_DLL
#define IMPORT_EXPORT_GETWINDOWSVERSION_DLL __declspec(dllexport)
#else
#define IMPORT_EXPORT_GETWINDOWSVERSION_DLL __declspec(dllimport)
#endif

#define OS_ERROR							-1
#define OS_WIN32_WINDOWS_NT_3_51			0
#define OS_WIN32_WINDOWS_NT_4_0				1
#define OS_WIN32_WINDOWS_95					2
#define OS_WIN32_WINDOWS_98					3
#define OS_WIN32_WINDOWS_Me					4
#define OS_WIN32_WINDOWS_2000				5
#define OS_WIN32_WINDOWS_XP					6
#define OS_WIN32_WINDOWS_XP_64				7
#define OS_WIN32_WINDOWS_SERVER_2003		8
#define OS_WIN32_WINDOWS_SERVER_2003_R2		9
#define OS_WIN32_WINDOWS_SERVER_2003_64		10
#define OS_WIN32_WINDOWS_VISTA				11
#define OS_WIN32_WINDOWS_VISTA_64			12
#define OS_WIN32_WINDOWS_SERVER_2008		13
#define OS_WIN32_WINDOWS_SERVER_2008_64		14
#define OS_WIN32_WINDOWS_SEVEN				15
#define OS_WIN32_WINDOWS_SEVEN_64			16
#define OS_WIN32_WINDOWS_SEVEN_SERVER		17
#define OS_WIN32_WINDOWS_SEVEN_SERVER_64	18
#define OS_WIN32_WINDOWS_EIGHT				19
#define OS_WIN32_WINDOWS_EIGHT_64			20
#define OS_WIN32_WINDOWS_EIGHT_SERVER		21
#define OS_WIN32_WINDOWS_EIGHT_SERVER_64	22
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_GETWINDOWSVERSION_DLL int GetWindowsVersion(void);
IMPORT_EXPORT_GETWINDOWSVERSION_DLL BOOL IsWow64(void);
/*-----------------------------------------------------------------------------------*/
#endif /* __GETWINDOWSVERSION_H__ */
