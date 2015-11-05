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

#ifndef __SCI_PATH_H__
#define __SCI_PATH_H__

#include <wchar.h>

#include "dynlib_ast.h"

AST_IMPEXP wchar_t* getSCIW(void);
AST_IMPEXP char* getSCI(void);

AST_IMPEXP void setSCIW(const wchar_t* _sci_path);
AST_IMPEXP void setSCI(const char* _sci_path);

AST_IMPEXP wchar_t* computeSCIW();
AST_IMPEXP char* computeSCI();

AST_IMPEXP wchar_t* getenvSCIW(void);
AST_IMPEXP char* getenvSCI(void);

AST_IMPEXP void putenvSCIW(const wchar_t* _sci_path);
AST_IMPEXP void putenvSCI(const char* _sci_path);

AST_IMPEXP void defineSCI();

#endif // ! __SCI_PATH_H__