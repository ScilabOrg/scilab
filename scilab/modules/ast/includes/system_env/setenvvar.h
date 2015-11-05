/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "stdio.h"
#include "PATH_MAX.h"

#include "dynlib_ast.h"

/* DIR_SEPARATOR : Under Windows by default is \ */
#ifdef _MSC_VER
#define DIR_SEPARATOR "\\"
#define DIR_SEPARATORW L"\\"
#else
#define DIR_SEPARATOR "/"
#define DIR_SEPARATORW L"/"
#endif

#define UNIX_SEPATATOR '/'
#define WINDOWS_SEPATATOR '\\'

AST_IMPEXP bool IsTheGoodShell(void);
AST_IMPEXP bool Set_Shell(void);
AST_IMPEXP void Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void);
AST_IMPEXP void SciEnvForWindows(void);
AST_IMPEXP void SetScilabEnvironmentVariables(char *DefaultSCIPATH);
AST_IMPEXP bool convertSlash(const char *path_in, char *path_out, bool slashToAntislash);
AST_IMPEXP bool AntislashToSlash(const char *pathwindows, char *pathunix);
AST_IMPEXP bool SlashToAntislash(const char *pathwindows, char *pathunix);
AST_IMPEXP bool isdir(const char * path);
AST_IMPEXP bool createdirectory(const char *path);
AST_IMPEXP bool isDrive(const char *strname);
AST_IMPEXP void SetScilabEnvironment(void);
AST_IMPEXP void SetScilabVariables(void);
AST_IMPEXP int SciEnvForOthers(void);

