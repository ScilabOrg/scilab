/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __SCI_HOME_H__
#define __SCI_HOME_H__

#include <wchar.h>
#include "BOOL.h"

#include "dynlib_ast.h"

AST_IMPEXP wchar_t *getSCIHOMEW(void);
AST_IMPEXP char *getSCIHOME(void);

AST_IMPEXP void setSCIHOMEW(const wchar_t* _sci_path);
AST_IMPEXP void setSCIHOME(const char* _sci_path);

AST_IMPEXP wchar_t* computeSCIHOMEW();
AST_IMPEXP char* computeSCIHOME();

AST_IMPEXP wchar_t* getenvSCIHOMEW(void);
AST_IMPEXP char* getenvSCIHOME(void);

AST_IMPEXP void putenvSCIHOMEW(const wchar_t* _sci_path);
AST_IMPEXP void putenvSCIHOME(const char* _sci_path);

AST_IMPEXP void defineSCIHOME();

#endif // ! __SCI_HOME_H__
