/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __CONFIGVARIABLE_INTERFACE_H__
#define __CONFIGVARIABLE_INTERFACE_H__

#include <string.h>
#include <wchar.h>

#include "dynlib_ast.h"

#define PROMPTMODE_NORMAL   0   //show new values but not commands
#define PROMPTMODE_SILENT   -1  //hide all
#define PROMPTMODE_PROMPT   2   //show all
#define PROMPTMODE_EXEC     1   //show all
#define PROMPTMODE_EXEC3    3   //show all
#define PROMPTMODE_STEP     4   //later ...
#define PROMPTMODE_STEP7    7   //later ...

#define SILENT_ERROR        1
#define VERBOSE_ERROR       0

typedef enum
{
    SCILAB_API = 1, /* Scilab is launch as an API */
    SCILAB_STD, /* The standard Scilab (gui, plot ...) */
    SCILAB_NW, /* Scilab in command line with the plots */
    SCILAB_NWNI /* Scilab in command line without any graphics. What not mandataroy here */
} scilabMode;

AST_IMPEXP int getSimpMode(void);
AST_IMPEXP void setSimpMode(int);
AST_IMPEXP int getStartProcessing(void);
AST_IMPEXP int getEndProcessing(void);

AST_IMPEXP void setFormatSize(int _iFormatSize);
AST_IMPEXP int getFormatSize(void);
AST_IMPEXP void setFormatMode(int _iFormatMode);
AST_IMPEXP int getFormatMode(void);
AST_IMPEXP void setConsoleWidth(int _iConsoleWidth);
AST_IMPEXP int getConsoleWidth(void);
AST_IMPEXP void setConsoleLines(int _iConsoleLines);
AST_IMPEXP int getConsoleLines(void);

AST_IMPEXP int getPromptMode(void);
AST_IMPEXP void setPromptMode(int _iMode);
AST_IMPEXP int isPromptShow(void);
AST_IMPEXP int getSilentError(void);
AST_IMPEXP void setSilentError(int _iSilent);

AST_IMPEXP int getieee(void);
AST_IMPEXP void setieee(int);

AST_IMPEXP scilabMode getScilabMode(void);
AST_IMPEXP void setScilabMode(scilabMode newmode);
AST_IMPEXP const char * getScilabModeString(void);

AST_IMPEXP int getWarningMode(void);
AST_IMPEXP void setWarningMode(int _iMode);

AST_IMPEXP int checkReferenceModule(const wchar_t* _module);
AST_IMPEXP void addReferenceModule(const wchar_t* _module);
AST_IMPEXP void removeReferenceModule(const wchar_t* _module);

AST_IMPEXP int getForceQuit();

AST_IMPEXP int getEntryPointPosition(wchar_t* _pwstEntryPointName);
typedef void(*dynlib_ptr)(void);
AST_IMPEXP dynlib_ptr getEntryPointFromPosition(int position);

AST_IMPEXP int isEnableDebug();
AST_IMPEXP int isDebugInterrupted();

AST_IMPEXP int isExecutionBreak();
AST_IMPEXP void setExecutionBreak();
AST_IMPEXP void resetExecutionBreak();

AST_IMPEXP int setRecursionLimit(int);
#endif /* !__CONFIGVARIABLE_INTERFACE_H__ */
