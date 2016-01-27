/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __ADDFUNCTION_H__
#define __ADDFUNCTION_H__

#include "dynlib_ast.h"

AST_IMPEXP void addCStackFunction(const wchar_t* _wstName, OLDGW_FUNC _pFunc, const wchar_t* _wstModule);
AST_IMPEXP void addCFunction(const wchar_t* _wstName, GW_C_FUNC _pFunc, const wchar_t* _wstModule);
AST_IMPEXP void addMexFunction(const wchar_t* _wstName, MEXGW_FUNC _pFunc, const wchar_t* _wstModule);

#endif // __ADDFUNCTION_H__
