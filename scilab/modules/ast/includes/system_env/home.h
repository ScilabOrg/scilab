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

#ifndef __HOME_H__
#define __HOME_H__

#include <wchar.h>
#include "BOOL.h"

#include "dynlib_ast.h"

AST_IMPEXP wchar_t *getHOMEW(void);
AST_IMPEXP char *getHOME(void);

AST_IMPEXP void setHOMEW(const wchar_t* _sci_path);
AST_IMPEXP void setHOME(const char* _sci_path);

AST_IMPEXP wchar_t* computeHOMEW();
AST_IMPEXP char* computeHOME();

AST_IMPEXP wchar_t* getenvHOMEW(void);
AST_IMPEXP char* getenvHOME(void);

AST_IMPEXP void putenvHOMEW(const wchar_t* _sci_path);
AST_IMPEXP void putenvHOME(const char* _sci_path);

AST_IMPEXP void defineHOME();

#endif // ! __HOME_H__
