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
#include "c_gateway_prototype.h"

#ifdef __cplusplus
extern "C"
#endif
EXTERN_AST void addCFunction(wchar_t* _wstName, OLDGW_FUNC _pFunc, wchar_t* _wstModule);
EXTERN_AST void addMexFunction(wchar_t* _wstName, MEXGW_FUNC _pFunc, wchar_t* _wstModule);

#endif // __ADDFUNCTION_H__
